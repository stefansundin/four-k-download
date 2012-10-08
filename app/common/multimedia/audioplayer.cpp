/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/


#include "multimedia/audioplayer.h"
#include "openmedia/DTAudioSource.h"
#include "openmedia/DTAudioFormat.h"
#include "openmedia/DTAudioDataTimed.h"
#include <QTimerEvent>
#include <QCoreApplication>
#include <QMutex>
#include <QDebug>

using namespace Multimedia;
using namespace openmedia;


namespace
{
    const QEvent::Type EndReachedEventType = QEvent::Type(QEvent::User + 10);

    const unsigned int OutputFrequency = 44100;
    const unsigned int OutputChannels = 2;
    const unsigned int OutputChannelsLayout = 3;

    QMutex timeMutex;

} // Anonimous


AudioPlayer::AudioPlayer(QObject* parent) :
    Player(parent),
    m_volume(100),
    m_isMuted(false),
    m_audioStream(NULL),
    m_autoSystem(),
    m_system(portaudio::System::instance()),
    m_streamStartTime(-1),
    m_streamCurrentTime(-1),
    m_sourceStartTime(0),
    m_audioTimerId(-1)
{
}


AudioPlayer::~AudioPlayer()
{
    resetSource();
    m_system.terminate();
}


void AudioPlayer::resetSource(openmedia::av_source_ptr source)
{
    if (state() != Player::StopState)
    {
        if (m_audioStream)
        {
            if (m_audioStream->isActive())
                m_audioStream->stop();
            m_audioStream->close();

            delete m_audioStream;
            m_audioStream = NULL;
        }

        m_streamStartTime = -1;
        m_streamCurrentTime = -1;
        m_sourceStartTime = 0;

        m_audioSource.reset();
        m_audioResampler.reset();
        m_amplifyFilter.reset();

        if (m_audioTimerId != -1)
        {
            killTimer(m_audioTimerId);
            m_audioTimerId = -1;
        }
    }

    m_audioSource = source && source->audio() ? source : openmedia::av_source_ptr();

    if (m_audioSource && m_system.defaultOutputDevice() != m_system.nullDevice())
    {
        m_sourceStartTime = m_audioSource->audio()->get_current_pos();

        // resample and amplify

        audio_format_ptr resampleOutputFormat = audio_format::create(
                    OutputFrequency, DT_SAMPLE_FMT_FLT,
                    OutputChannels, OutputChannelsLayout);

        audio_format_ptr resampleInputFormat = audio_format::create(
                    m_audioSource->audio()->get_audio_format().get());

        m_audioResampler = audio_convert_utils::create_resample_convert(
                    resampleOutputFormat.get(), resampleInputFormat.get());

        m_amplifyFilter = audio_filters_utils::create_amplify(resampleOutputFormat.get());
        m_amplifyFilter->apply(media_settings::create("gain", m_volume / 100.0).get());

        // portaudio

        portaudio::DirectionSpecificStreamParameters streamOutputParams(
                    m_system.defaultOutputDevice(), OutputChannels, portaudio::FLOAT32, true,
                    m_system.defaultOutputDevice().defaultLowOutputLatency(),
                    NULL);

        portaudio::StreamParameters streamParams(
                    portaudio::DirectionSpecificStreamParameters::null(),
                    streamOutputParams, OutputFrequency, 0, paNoFlag);

        m_audioStream = new portaudio::MemFunCallbackStream<AudioPlayer>(streamParams, *this, &AudioPlayer::readAudio);

        setState(Player::PauseState);
    }
    else
    {
        setState(Player::StopState);
    }

    emit maxTimeChanged(maxTime());
    emit timeChanged(time(), true);
}


void AudioPlayer::play()
{
    if (state() != Player::PauseState)
        return;

    m_audioStream->start();
    m_audioTimerId = startTimer(25);

    setState(Player::PlayState);
}


void AudioPlayer::pause()
{
    if (state() != Player::PlayState)
        return;

    if (m_audioStream->isActive())
        m_audioStream->stop();
    m_sourceStartTime += (m_streamCurrentTime - m_streamStartTime) * 1000;
    m_streamCurrentTime = -1;
    m_streamStartTime = -1;

    if (m_audioTimerId != -1)
    {
        killTimer(m_audioTimerId);
        m_audioTimerId = -1;
    }

    setState(Player::PauseState);
}


qint64 AudioPlayer::minTime() const
{
    return 0;
}


qint64 AudioPlayer::maxTime() const
{
    if (state() == Player::StopState)
        return 0;

    return m_audioSource->audio()->get_duration();
}


qint64 AudioPlayer::time() const
{
    if (state() == Player::StopState)
        return 0;

    qint64 result = 0;

    timeMutex.lock();

    if (m_streamCurrentTime == -1 || m_streamStartTime == -1)
        result = m_sourceStartTime;
    else
        result = m_sourceStartTime + (m_streamCurrentTime - m_streamStartTime) * 1000;

    timeMutex.unlock();

    return qMin(result, maxTime());
}


void AudioPlayer::setTime(qint64 value)
{
    if (state() == Player::StopState || time() == value)
        return;

    if (value < minTime() || value > maxTime())
        value = minTime();

    if (m_audioStream->isActive())
        m_audioStream->abort();
    m_streamCurrentTime = -1;
    m_streamStartTime = -1;

    m_audioSource->seek(value);
    m_sourceStartTime = m_audioSource->audio()->get_current_pos();

    if (state() == Player::PlayState)
        m_audioStream->start();

    emit timeChanged(time(), true);
}


int AudioPlayer::minVolume() const
{
    return 0;
}


int AudioPlayer::maxVolume() const
{
    return 100;
}


int AudioPlayer::volume() const
{
    return m_volume;
}


void AudioPlayer::setVolume(int value)
{
    if (m_volume != value)
    {
        m_volume = value;

        if (m_amplifyFilter)
            m_amplifyFilter->apply(media_settings::create("gain", m_volume / 100.0).get());

        emit volumeChanged(volume());
    }
}


bool AudioPlayer::isMuted() const
{
    return m_isMuted;
}


void AudioPlayer::setMuted(bool value)
{
    if (m_isMuted != value)
    {
        m_isMuted = value;
        emit mutedChanged(volume());
    }
}


bool AudioPlayer::event(QEvent *event)
{
    if (event->type() == EndReachedEventType)
    {
        pause();
        setTime(minTime());

        emit completed();

        return true;
    }

    return QObject::event(event);
}


void AudioPlayer::timerEvent(QTimerEvent *event)
{
    if  (event->timerId() == m_audioTimerId)
    {
        emit timeChanged(time());
    }

    QObject::timerEvent(event);
}


int AudioPlayer::readAudio(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    if (!m_audioSource)
        return paAbort;

    timeMutex.lock();

    if (m_streamCurrentTime == -1)
        m_streamStartTime = timeInfo->currentTime;
    m_streamCurrentTime = timeInfo->currentTime;

    timeMutex.unlock();

    try
    {
        while (framesPerBuffer > (unsigned long)m_audioResampler->get_samples_count())
        {
            audio_data_timed_ptr t = m_audioSource->audio()->read_audio(1000.0 * framesPerBuffer / OutputFrequency);
            if (t && t->get_media_data())
            {
                audio_data_ptr audioData = t->get_media_data();
                if (audioData && audioData->get_samples_count())
                {
                    m_audioResampler->push_back(audioData.get());
                }
            }
            else
                break;
        }

        unsigned long samples = (std::min)(framesPerBuffer, (unsigned long)m_audioResampler->get_samples_count());
        audio_data_ptr convertedData = m_audioResampler->pop_front(samples);

        if (convertedData)
        {
            m_amplifyFilter->push_back(convertedData.get());
            convertedData = m_amplifyFilter->pop_front(samples);
        }

        if (convertedData)
            memcpy(outputBuffer, convertedData->get_raw_data(), convertedData->get_raw_size());

        if (samples < framesPerBuffer)
        {
            QCoreApplication::postEvent(this, new QEvent(EndReachedEventType));
            return paComplete;
        }

        return paContinue;
    }
    catch(...)
    {
        QCoreApplication::postEvent(this, new QEvent(EndReachedEventType));
        return paAbort;
    }
}
