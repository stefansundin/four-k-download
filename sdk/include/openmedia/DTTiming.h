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


/// \file   DTTiming.h

#ifndef _DTTIMING_H_INCLUDED_
#define _DTTIMING_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTPacket.h>

namespace openmedia {

/// \class  media_data_timed
/// \tparam MediaData
template <class MediaData>
class media_data_timed
{
public:
    typedef boost::shared_ptr<MediaData> MediaDataPtr;

public:
    media_data_timed();
    media_data_timed(MediaDataPtr & _MediaData, double _Duration, dt_float_t _TimeStamp);
    static boost::shared_ptr< media_data_timed<MediaData> > create(MediaDataPtr & _MediaData, double _Duration, dt_float_t _TimeStamp);

public:
    MediaDataPtr    get_media_data() const;
    
    double          get_duration() const;
    dt_float_t      get_time_stamp() const;
    
private:
    MediaDataPtr    m_MediaDataPtr;
    double          m_Duration;
    dt_float_t      m_TimeStamp;

};

template <class MediaData>
boost::shared_ptr< media_data_timed<MediaData> > media_data_timed<MediaData>::
create(MediaDataPtr & _MediaData, double _Duration, dt_float_t _TimeStamp)
{
    return boost::shared_ptr< media_data_timed<MediaData> >( new media_data_timed<MediaData>(_MediaData, _Duration, _TimeStamp) );
}

template <class MediaData>
media_data_timed<MediaData>::media_data_timed()
:
m_MediaDataPtr((MediaData*)NULL),
m_Duration(0.0),
m_TimeStamp(0)
{
};

template <class MediaData>
media_data_timed<MediaData>::media_data_timed(typename media_data_timed<MediaData>::MediaDataPtr & _MediaData,
                                                     double _Duration,
                                                     dt_float_t _TimeStamp
                                                     )
:
m_MediaDataPtr(_MediaData),
m_Duration(_Duration),
m_TimeStamp(_TimeStamp)
{
};

template <class MediaData>
typename media_data_timed<MediaData>::MediaDataPtr    media_data_timed<MediaData>::get_media_data() const
{
    return m_MediaDataPtr;
}

template <class MediaData>
double media_data_timed<MediaData>::get_duration() const
{
    return m_Duration;
}

template <class MediaData>
dt_float_t media_data_timed<MediaData>::get_time_stamp() const
{
    return m_TimeStamp;
}
    
} // namespace openmedia

#endif // #ifndef _DTTIMING_H_INCLUDED_

