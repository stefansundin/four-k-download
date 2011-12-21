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


#ifndef _DT_DOWNLOADERYOUTUBETOOLS_H_
#define _DT_DOWNLOADERYOUTUBETOOLS_H_

#include <vector>
#include <string>
#include <openmedia/DTMediaDownloader.h>

namespace openmedia { namespace downloader { namespace youtube {
    
bool receive_youtube_info_impl(const std::string & Url,
                          const std::string & ConnectionType,
                          const std::string & ContentType,
                          const std::string & UserAgent,
                          const std::string & Content,
                          std::string & Result
                          );

bool receive_youtube_info_impl2(const std::string & UserAgent, const ::std::vector< std::string > & Ids, std::string & Result);

struct youtube_desc
{
public:
    youtube_desc(const std::string & Username, const std::string & Id, const std::string & TitleUtf8, const std::wstring & TitleW) : username_(Username), 
        id_(Id),
        title_(TitleUtf8),
        titlew_(TitleW)
    {
    }

public:
    const std::string & username() const { return username_; }
    const std::string & id() const { return id_; }
    const std::string & title_utf8() const { return title_; }
    const std::wstring & title() const { return titlew_; }

private:
    std::string username_;
    std::string id_;
    std::string title_;
    std::wstring titlew_;

};

bool receive_youtube_info(const std::string & UserAgent, const ::std::vector< std::string > & Ids, std::vector<youtube_desc> & Result);

}
}
}

#endif
