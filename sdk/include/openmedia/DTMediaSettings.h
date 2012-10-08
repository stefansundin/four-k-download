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


#ifndef _OPENMEDIA_DT_MEDIA_SETTINGS_H_INCLUDED_
#define _OPENMEDIA_DT_MEDIA_SETTINGS_H_INCLUDED_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <string>

namespace openmedia {

class media_settings_impl;

class media_settings;
typedef boost::shared_ptr<media_settings> media_settings_ptr;

/// \class media_settings
class _OPENMEDIASDK_API media_settings: public boost::enable_shared_from_this<media_settings>
{
public:
    ~media_settings();
    
    std::string get_param(const std::string & ParamName) const;
    boost::optional<std::string> get_param_optional(const std::string & ParamName) const;
    
    template <class T>
    boost::optional<T> get_param_optional(const std::string & ParamName) const;

    bool exist(const std::string & ParamName) const;
    
    template <class T>
    T get_param(const std::string & ParamName) const;

    template <class T>
    T get_param_or(const std::string & ParamName, T defaultVal) const;

    media_settings_ptr set_param(const std::string & ParamName, const std::string & ParamValue);
    media_settings_ptr set_param(const std::pair<std::string, std::string> & Param);
    media_settings_ptr set_param(const std::string & ParamName, const char * const ParamValue);
    
    template <class T>
    media_settings_ptr set_param(const std::string & ParamName, T ParamValue);

    media_settings_ptr apply_from(const media_settings & settings);
    media_settings_ptr apply_from(const media_settings_ptr & settings);
    
    media_settings_ptr duplicate() const;
    media_settings_ptr clone() const;
   
public:
    static media_settings_ptr create();
    static media_settings_ptr create(media_settings_ptr settings);

    template <class T>
    static media_settings_ptr create(const std::string & ParamName, T ParamValue);

private:
    media_settings();
    friend boost::shared_ptr<media_settings> boost::make_shared<media_settings>();
    media_settings_impl * impl_;
};

template <class T>
inline media_settings_ptr media_settings::create(const std::string & ParamName, T ParamValue)
{
    media_settings_ptr s = media_settings::create();
    s->set_param(ParamName, ParamValue);
    return s;    
}


inline media_settings_ptr media_settings::set_param(const std::string & ParamName, const char * const ParamValue)
{
    return set_param(ParamName, std::string(ParamValue));
}

inline media_settings_ptr media_settings::set_param(const std::pair<std::string, std::string> & Param)
{
    return set_param(Param.first, Param.second);
}

template <class T>
inline media_settings_ptr media_settings::set_param(const std::string & ParamName, T ParamValue)
{
    return set_param(ParamName, boost::lexical_cast<std::string>(ParamValue));
}

template <class T>
T media_settings::get_param(const std::string & ParamName) const
{
    return boost::lexical_cast<T>(get_param(ParamName));
}

template <>
inline double media_settings::get_param<double>(const std::string & ParamName) const
{
    try 
    {
        return boost::lexical_cast<double>(get_param(ParamName));
    }
    catch(boost::bad_lexical_cast &)
    {
        std::string s = get_param(ParamName);
        boost::algorithm::replace_all(s, ",", ".");
        return boost::lexical_cast<double>(s);
    }
}

template <class T>
boost::optional<T> media_settings::get_param_optional(const std::string & ParamName) const
{
    if (exist(ParamName))
        return get_param<T>(ParamName);
    else
        return boost::optional<T>();
}


template <class T>
T media_settings::get_param_or(const std::string & ParamName, T defaultVal) const
{
    const std::string val = get_param(ParamName);
    if (val.empty())
        return defaultVal;
    else
        return boost::lexical_cast<T>(val);
}

}

#endif
