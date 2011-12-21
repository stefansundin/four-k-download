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


#include <openmedia/DTScript.h>
#include "DTPythonScript.h"
#include "DTQtScript.h"
#include <openmedia/DTAssert.h>

namespace openmedia { namespace script { 

script_ptr create(scriptType Type, const std::string & Program)
{
    switch(Type)
    {
    case scriptPython:
#if defined(DT_HAVE_PYTHON_SCRIPT) && (1 == DT_HAVE_PYTHON_SCRIPT) && !defined(DT_HAVE_PYTHON_SCRIPT_DISABLED)
        return script_ptr( new python::python_script(Program) );
#endif
        break;
    case scriptQt:
#if defined(DT_HAVE_QT_SCRIPT) && (1 == DT_HAVE_QT_SCRIPT) && !defined(DT_HAVE_QT_SCRIPT_DISABLED)
        return script_ptr( new qt::qt_script(Program) );
#endif
    default:
        break;
    }

    DT_ASSERT(false);
    BOOST_THROW_EXCEPTION(script::error());
    return script_ptr();
}

} }
