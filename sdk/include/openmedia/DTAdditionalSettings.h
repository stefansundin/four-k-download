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


/// \file   DTAdditionalSettings.h

#ifndef _DTADDITIONALSETTINGS_H_INCLUDED_
#define _DTADDITIONALSETTINGS_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include <openmedia/DTDeclareImpl.h>

namespace openmedia {

/// \class  additional_settings
class _OPENMEDIASDK_API additional_settings: interface_base
{
public:
    struct invalid_setting : virtual errors::invalid_argument{};

    DT_DECLARE_IMPL2(additional_settings);
public:
    additional_settings();
    virtual     ~additional_settings();
    void        add_setting(const std::string & _Name, const std::string & _Value);
    size_t      size() const;
    void        get_at(size_t _Index, std::string & _Name, std::string & _Value) const;
    const std::string & find(const std::string & Name) const;
    
private:
    Impl * m_pImpl;
};

}

#endif // #ifndef _DTADDITIONALSETTINGS_H_INCLUDED_

