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

/// \file   DTAdditionalSettings.cpp

#include <openmedia/DTAdditionalSettings.h>

#include <map>
#include <vector>

namespace openmedia {
   
/// \class  additional_settings::Impl
class additional_settings::Impl: impl_base
{
public:
    void        add_setting(const std::string & _Name, const std::string & _Value);
    size_t      size() const;
    void        get_at(size_t _Index, std::string & _Name, std::string & _Value) const;
    const std::string & find(const std::string & Name) const;

private:
    typedef std::pair<std::string, std::string> StringPair; 
    typedef std::vector<StringPair> StringPairVector;

    StringPairVector m_Vector;
};

void additional_settings::Impl::add_setting(const std::string & _Name, const std::string & _Value)
{
    StringPair val(_Name, _Value);
    m_Vector.push_back(val);
}

size_t additional_settings::Impl::size() const
{
    return m_Vector.size();
}

void additional_settings::Impl::get_at(size_t _Index, std::string & _Name, std::string & _Value) const
{
    DT_ASSERT(_Index < m_Vector.size());
    StringPair val = m_Vector.at(_Index);
    _Name = val.first;
    _Value = val.second;
}


const std::string & additional_settings::Impl::find(const std::string & Name) const
{
    static const std::string InvalidResult = "";
    for (StringPairVector::const_iterator i = m_Vector.begin(); i != m_Vector.end(); ++i)
        if (i->first == Name)
            return i->second;

    BOOST_THROW_EXCEPTION(invalid_setting());
    DT_IF_DISABLE_EXCEPTIONS(return InvalidResult);
}

///

additional_settings::additional_settings(): m_pImpl( new additional_settings::Impl() )
{
}

additional_settings::~additional_settings()
{
    delete m_pImpl;
}

void additional_settings::add_setting(const std::string & _Name, const std::string & _Value)
{
    return impl()->add_setting(_Name, _Value);
}

size_t additional_settings::size() const
{
    return impl()->size();
}

void additional_settings::get_at(size_t _Index, std::string & _Name, std::string & _Value) const
{
    return impl()->get_at(_Index, _Name, _Value);
}

} // namespace openmedia
