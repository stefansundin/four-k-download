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


/// \file   DTAudioFormatImpl.h

#ifndef _DTAUDIOFORMATIMPL_H_INCLUDED_
#define _DTAUDIOFORMATIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioUtils.h>
#include <openmedia/DTTiming.h>

namespace openmedia {

/// \class  audio_format::Impl
class audio_format::Impl : public impl_base
{
public:
    Impl(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount, dt_channel_layout_t _ChannelLayout);

public:
	int					get_sample_rate() const;
    dt_sample_format_t  get_sample_format() const;
    dt_channel_layout_t get_channel_layout() const;
    int                 get_channels_count() const;
    int                 get_bits_per_sample() const;
    ptrdiff_t           get_sample_align() const;

private:
    dt_sample_format_t  m_SampleFormat;
    dt_channel_layout_t m_ChannelLayout;
    int                 m_ChannelsCount;
    int                 m_SampleRate;
};

inline audio_format::Impl::Impl(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount, dt_channel_layout_t _ChannelLayout):
        m_SampleFormat(_SampleFormat),
        m_ChannelLayout(_ChannelLayout),
        m_ChannelsCount(_ChannelsCount),
        m_SampleRate(_SampleRate)
{
#if 0
    if (0 == _SampleRate)
    {
        DT_ASSERT(false);
        BOOST_THROW_EXCEPTION(audio_format::invalid_format());
    }
#endif
}

inline int audio_format::Impl::get_sample_rate() const
{
    return m_SampleRate;
}

inline dt_sample_format_t audio_format::Impl::get_sample_format() const
{
    return m_SampleFormat;
}

inline dt_channel_layout_t audio_format::Impl::get_channel_layout() const
{
    return m_ChannelLayout;
}

inline int audio_format::Impl::get_channels_count() const
{
    return m_ChannelsCount;
}

inline int audio_format::Impl::get_bits_per_sample() const
{
    return utils::audio::sample_fmt_to_bps(m_SampleFormat);
}

inline ptrdiff_t audio_format::Impl::get_sample_align() const
{
    return utils::audio::bits_to_bytes(this->get_bits_per_sample()) * this->get_channels_count();
}

} // namespace openmedia

#endif // #ifndef _DTAUDIOFORMATIMPL_H_INCLUDED_
