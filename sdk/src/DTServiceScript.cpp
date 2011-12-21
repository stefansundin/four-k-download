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


#include <openmedia/DTHeaders.h>

#define BOOST_THREAD_USE_LIB
#include "DTServiceScript.h"
#include <openmedia/DTMediaDownloader.h>

#include <string>
#include <map>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>

namespace openmedia { namespace downloader {

typedef ::std::pair<unsigned int, ::std::string > id_string_pair;
typedef ::std::vector < id_string_pair > id_string_pair_array;
typedef std::map<media_site_type_t, id_string_pair_array> services_map;

namespace {

services_map services_;

}

boost::mutex service_script_get_lock_;

boost::mutex & service_script_get_lock()
{
    return service_script_get_lock_;
}

service_script_info_list service_script_info_list_;

const service_script_info_list & get_service_script_info_list()
{
    return service_script_info_list_;
}

void set_service_script_list(service_script_info_list & _List)
{
    service_script_info_list_ = _List;
}

namespace {
    service_script_info empty_service_script_info;
}

const service_script_info & find_service_script_info(media_site_type_t MediaSite, const unsigned int Id)
{
    const service_script_info_list & infoList =  get_service_script_info_list();
    BOOST_FOREACH(const service_script_info & i, infoList)
    {
        if (i.mediaSite == MediaSite && i.id == Id)
            return i;
    }
    return empty_service_script_info;
}

std::string get_service_script_impl(media_site_type_t MediaSite, unsigned int Id, bool r)
{
    do 
    {
        boost::mutex & lock = service_script_get_lock();
        boost::mutex::scoped_lock lk(lock);

        services_map::iterator i = services_.find(MediaSite);
        if (services_.end() == i)
            break;

        id_string_pair_array & ar = (*i).second;

        BOOST_FOREACH(id_string_pair & v, ar)
        {
            if (Id == v.first)
            {
                return v.second;
            }
        }
        break;
    } 
    while((false));

    const service_script_info & info = find_service_script_info(MediaSite, Id);
    
    if (mediaSiteNull == info.mediaSite)
        return "";
    else
    {
        set_service_script_url(info.mediaSite, info.id, info.url); 
        return (!r) ? get_service_script_impl(MediaSite, Id, true) : ("");
    }
}

std::string get_service_script(media_site_type_t MediaSite, const unsigned int Id)
{
    return get_service_script_impl(MediaSite, Id, false);
}

void set_service_script(media_site_type_t MediaSite, const unsigned int Id, const std::string & Script)
{
    boost::mutex & lock = service_script_get_lock();
    boost::mutex::scoped_lock lk(lock);

    services_map::iterator i = services_.find(MediaSite);
    
    if (i == services_.end())
    {
        services_.insert( services_map::value_type( MediaSite, id_string_pair_array() ) );
        i = services_.find(MediaSite);
    }

    id_string_pair_array & ar = (*i).second;
    
    BOOST_FOREACH(id_string_pair & v, ar)
    {
        if (Id == v.first)
        {
            //replace
            v.second = Script;
            return;
        }
    }
    ar.push_back( id_string_pair(Id, Script) );
    return;
}

} }
