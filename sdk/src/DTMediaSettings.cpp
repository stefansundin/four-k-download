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



// precompiled header begin
#include "DTHeadersMedia.h"
// precompiled header end
#include <map>
#include <openmedia/DTMediaSettings.h>

namespace openmedia {

media_settings_ptr media_settings::create()
{
    return boost::make_shared<media_settings>();
}

class media_settings_impl
{
public:
    std::string get_param(const std::string & ParamName) const;
    void set_param(const std::string & ParamName, const std::string & ParamValue);
    void apply_form(const media_settings_impl & rVal);
    bool exist(const std::string & ParamName) const;

    void set_owner(media_settings * owner)
    { 
        owner_ = owner;
    }

private:
    typedef std::map<std::string, std::string> ParamsMap;
    ParamsMap params_;
    media_settings * owner_;

};

void media_settings_impl::apply_form(const media_settings_impl & rVal)
{
    for (ParamsMap::const_iterator i = rVal.params_.begin(); 
        i != rVal.params_.end(); ++i)
    {
        set_param(i->first, i->second);
    }
}

std::string media_settings_impl::get_param(const std::string & ParamName) const
{
    ParamsMap::const_iterator i = params_.find(ParamName);
    if (i == params_.end())
        return "";
    else
        return i->second;
}

bool media_settings_impl::exist(const std::string & ParamName) const
{
    return params_.find(ParamName) != params_.end();
}

void media_settings_impl::set_param(const std::string & ParamName, const std::string & ParamValue)
{
    ParamsMap::iterator i = params_.find(ParamName);
    if (i == params_.end())
    {
        params_.insert( ParamsMap::value_type(ParamName, ParamValue) );
    }
    else
    {
        i->second = ParamValue;
    }
}

media_settings::media_settings() : impl_( new media_settings_impl() )
{
    impl_->set_owner(this);
}

media_settings::~media_settings()
{
    delete impl_;
}

std::string media_settings::get_param(const std::string & ParamName) const
{
    return impl_->get_param(ParamName);
}

media_settings_ptr media_settings::set_param(const std::string & ParamName, const std::string & ParamValue)
{
    impl_->set_param(ParamName, ParamValue);
    return shared_from_this();
}

media_settings_ptr media_settings::apply_from(const media_settings & settings)
{
    impl_->apply_form(*settings.impl_);
    return shared_from_this();
}

media_settings_ptr media_settings::apply_from(const media_settings_ptr & settings)
{
    if (settings)
        return apply_from(*(settings.get()));
    else
        return shared_from_this();
}

media_settings_ptr media_settings::duplicate() const
{
    media_settings_ptr dup = media_settings::create();
    dup->apply_from(*this);
    return dup;    
}

media_settings_ptr media_settings::clone() const
{
    return this->duplicate();
}

boost::optional<std::string> media_settings::get_param_optional(const std::string & ParamName) const
{
    if (impl_->exist(ParamName))    
        return impl_->get_param(ParamName);
    else
        return boost::optional<std::string>();
}

bool media_settings::exist(const std::string & ParamName) const
{
    return impl_->exist(ParamName);    
}

media_settings_ptr media_settings::create(media_settings_ptr settings)
{
    media_settings_ptr newSettings = media_settings::create();
    newSettings->apply_from(settings);
    return newSettings;
}




}
