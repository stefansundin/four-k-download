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


#include <openmedia/DTConfig.h>

#if defined(DT_HAVE_PYTHON_SCRIPT) && (0 != DT_HAVE_PYTHON_SCRIPT)

#define BOOST_THREAD_USE_LIB
#include "DTPythonScript.h"
#include "DTScriptImpl.h"

#define Py_NO_ENABLE_SHARED
#if (defined(_WIN32) || defined(__WIN32__))
#include <Python.h>
#else
#include <python2.7/Python.h>
#endif

#include <string>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#ifdef _MSC_VER
#pragma comment(lib, "libpython27.a")
#endif

namespace openmedia { namespace script { namespace python {

boost::mutex pythonGuard;
boost::mutex pythonStateGuard;
bool pythonRun = false;
boost::thread::id pythonThreadId;

class python_script_impl : public script::Impl
{
public:
    python_script_impl(const ::std::string & PythonScript);
    virtual ~python_script_impl();

public:
    virtual std::string execute(const ::std::string & Name, const params & Params);

private:
    std::string propgram_;
    boost::unique_lock< boost::mutex > lock_;

};

python_script_impl::python_script_impl(const ::std::string & PythonScript):
propgram_(PythonScript)
{
    {
        boost::lock_guard< boost::mutex > lock(pythonStateGuard);
        if (pythonRun)
        {
            if (pythonThreadId == boost::this_thread::get_id())
            {
                DT_ASSERT(false);
                throw script::error();
            }
        }
    }

    lock_ = boost::unique_lock< boost::mutex >(pythonGuard);
    //boost::algorithm::replace_all(propgram_, "\r", "");
    
    try 
    {
        if (Py_IsInitialized())
        {
            throw ::openmedia::script::script::error();                                
        }

        Py_Initialize();
        int res;
        if ((res = PyRun_SimpleString((propgram_ + "\n").c_str())) < 0)
        {
            Py_Finalize();
            throw ::openmedia::script::script::error();
        }

        {
            boost::lock_guard< boost::mutex > lock(pythonStateGuard);
            pythonThreadId = boost::this_thread::get_id();
            pythonRun = true;
        }
    }
    catch(::openmedia::script::script::error &)
    {
        throw;
    }
    catch(...)
    {
        throw ::openmedia::script::script::error();
    }
}

python_script_impl::~python_script_impl()
{
    Py_Finalize();

    {
        boost::lock_guard< boost::mutex > lock(pythonStateGuard);
        pythonThreadId = boost::thread::id();
        pythonRun = false;
    }

}

std::string python_script_impl::execute(const ::std::string & Name, const params & Params)
{
    std::stringstream executeQuery;

    executeQuery << "result_val = " << Name << "({";

    bool f = true;
    BOOST_FOREACH( const string_string_pair & p, Params )
    {
        if (f)
        {
            f = !f;
        }
        else
        {
            executeQuery << ", ";        
        }

        executeQuery << "\"" << p.first << "\"" << ":\"" << p.second << "\"";
    }

    executeQuery << "})\n\r";

    if (PyRun_SimpleString( executeQuery.str().c_str() ) < 0)
        throw ::openmedia::script::script::error();

    PyObject * module = PyImport_AddModule("__main__");
    if (!module)
        throw ::openmedia::script::script::error();

    PyObject * dictionary = PyModule_GetDict(module);
    if (!module)
        throw ::openmedia::script::script::error();

    PyObject * result = PyDict_GetItemString(dictionary, "result_val");  
//    PyString_Check(result);
    const std::string res = PyString_AsString(result);
    return res;
}

python_script::python_script(const ::std::string & PythonScript) 
: script( new python_script_impl(PythonScript) )
{
}

} } }

#endif // #if defined(DT_HAVE_PYTHON_SCRIPT) && (0 != DT_HAVE_PYTHON_SCRIPT)
