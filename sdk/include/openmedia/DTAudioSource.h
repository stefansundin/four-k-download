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


#ifndef _OPENMEDIA_DT_AUDIO_SOURCE_H_INCLUDED_DC5C0090
#define _OPENMEDIA_DT_AUDIO_SOURCE_H_INCLUDED_DC5C0090

#include <boost/shared_ptr.hpp>
#include <openmedia/DTMediaSource.h>

namespace openmedia {

class audio_data;
template <class MediaData> class media_data_timed;
typedef media_data_timed<audio_data> audio_data_timed;
typedef boost::shared_ptr<audio_data_timed> audio_data_timed_ptr;

class audio_format;
typedef boost::shared_ptr<audio_format> audio_format_ptr;

/// \class  audio_source
class audio_source : public media_source
{
public:
    virtual audio_data_timed_ptr read_audio(double duration) = 0;
    virtual audio_format_ptr get_audio_format() const = 0;
};

} // namespace openmedia

#endif
