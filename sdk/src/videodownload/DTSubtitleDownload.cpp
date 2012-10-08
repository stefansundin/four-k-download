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


#include "DTHeadersVideoDownload.h"

#include "DTSubtitleDownload.h"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include "../DTDownloaderUtils.h"

namespace openmedia {
namespace downloader {

namespace 
{
std::string format_time(double seconds)
{
    boost::posix_time::millisec m( static_cast<boost::uint64_t>(1000.0 * seconds) );
    std::stringstream s;
    s << std::setfill('0') << std::setw(2);
    s << m.hours() << ":";
    s << std::setfill('0') << std::setw(2);
    s << m.minutes() << ":";
    s << std::setfill('0') << std::setw(2);
    s << m.seconds() << ",";
    s << std::setfill('0') << std::setw(3);
    s << static_cast<unsigned int>( 1.0 * m.fractional_seconds() / (1.0*m.ticks_per_second()/1000) );
    return s.str();
}
}

void subtitles_to_srt(const std::vector<transcript_element> & subtitles, std::ostream & strm)
{
    size_t count = 0;
    BOOST_FOREACH(const transcript_element & elm, subtitles)
    {
        count++;
        strm << count << "\r\n";
        strm << format_time(elm.start) << " --> " << format_time(elm.start + elm.duration) << "\r\n";
        strm << elm.text << "\r\n\r\n";
    }
}

}
}
