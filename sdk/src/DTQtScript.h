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


/// \file   DTQtScript.h

#ifndef _DTQTSCRIPT_H_INCLUDED_
#define _DTQTSCRIPT_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTTypes.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>

#include <openmedia/DTScript.h>

namespace openmedia { namespace script { namespace qt {

class qt_script : public script
{
public:
    qt_script(const ::std::string & QtScript);
};

} } }

#endif //#ifndef _DTQTSCRIPT_H_INCLUDED_
