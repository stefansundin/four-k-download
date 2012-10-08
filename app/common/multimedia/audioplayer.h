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


#ifndef MILTIMEDIA_AUDIOPLAYER_H
#define MILTIMEDIA_AUDIOPLAYER_H

#include <QObject>
#include <QMetaType>
#include "multimedia/player.h"
#include "multimedia/metatypes.h"
#include "openmedia/DTAVSource.h"
#include "openmedia/DTAudioConvert.h"
#include "openmedia/DTAudioFilters.h"
#include "portaudiocpp/PortAudioCpp.hxx"

namespace Multimedia
{

class AudioPlayer : public Player
{
    Q_OBJECT    
    Q_PROPERTY(qint64 minTime READ minTime NOTIFY fakeSignal)
    Q_PROPERTY(qint64 maxTime READ maxTime NOTIFY maxTimeChanged)
    Q_PROPERTY(qint64 time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(int minVolume READ minVolume NOTIFY fakeSignal)
    Q_PROPERTY(int maxVolume READ maxVolume NOTIFY fakeSignal)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)

public:
    explicit AudioPlayer(QObject* parent = 0);
    virtual ~AudioPlayer();

    qint64 minTime() const;
    qint64 maxTime() const;
    qint64 time() const;
    void setTime(qint64 value);
    int minVolume() const;
    int maxVolume() const;
    int volume() const;
    void setVolume(int value);
    bool isMuted() const;
    void setMuted(bool value);
    
    virtual void resetSource(openmedia::av_source_ptr source = openmedia::av_source_ptr());

public slots:
    virtual void play();
    virtual void pause();

signals:
    void maxTimeChanged(qint64 value);
    void timeChanged(qint64 value, bool isDeep = false);
    void volumeChanged(int value);
    void mutedChanged(bool value);
    void fakeSignal(); // Disable qml warnings
    void completed();

protected:
    bool event(QEvent *event);
    void timerEvent(QTimerEvent *event);
    int  readAudio(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);

private:
    int m_volume;
    bool m_isMuted;
    openmedia::av_source_ptr m_audioSource;
    openmedia::audio_convert_ptr m_audioResampler;
    openmedia::audio_filter_ptr m_amplifyFilter;
    portaudio::MemFunCallbackStream<AudioPlayer>* m_audioStream;
    portaudio::AutoSystem m_autoSystem;
    portaudio::System& m_system;
    double m_streamStartTime;
    double m_streamCurrentTime;
    qint64 m_sourceStartTime;
    int m_audioTimerId;
};

} // Multimedia

#endif // MILTIMEDIA_AUDIOPLAYER_H
