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

#if defined(DT_HAVE_QT_SCRIPT) && (1 == DT_HAVE_QT_SCRIPT) && !defined(DT_HAVE_QT_SCRIPT_DISABLED)

#if defined(linux) || defined(__linux) || defined(__linux__)
#include <QtScript/qscriptengine.h>
#include <QtGui/qapplication.h>
#include <QtCore/qcoreapplication.h>
#else
#include <QtScript/QScriptEngine.h>
#include <QtGui/QApplication.h>
#include <QtCore/QCoreApplication.h>
#endif
#include <string>
#include <iostream>

#include <boost/foreach.hpp>

#include "DTQtScript.h"
#include "DTScriptImpl.h"

namespace openmedia { namespace script { namespace qt {

class qt_script_impl : public script::Impl
{
public:
    qt_script_impl(const ::std::string & _QtScript);
    virtual ~qt_script_impl();

public:
    virtual std::string execute(const ::std::string & Name, const params & Params);

private:
    std::string program_;
    QScriptEngine * engine_;
    static QCoreApplication * qtApplication_;
};

QCoreApplication * qt_script_impl::qtApplication_ = NULL;

qt_script_impl::qt_script_impl(const std::string & _QtScript) : program_(_QtScript)
{
    if (!QCoreApplication::instance() && !QApplication::instance())
    {
        int argc = 1;
        char * argv[1] = {"openmedia"};
        qtApplication_ = new QCoreApplication(argc, argv); 
    }

    engine_ = new QScriptEngine();
}

qt_script_impl::~qt_script_impl()
{

}

std::string qt_script_impl::execute(const ::std::string & Name, const params & Params)
{
    ::std::stringstream args;
    bool f = true;
    args << "({";
    BOOST_FOREACH( const string_string_pair & p, Params )
    {
        if (f)
        {
            f = false;
        }
        else
        {
            args << ", ";        
        }
        args << p.first << ":" << "\"" << p.second << "\"";
        
        //QString name(p.first.c_str());
        //QScriptValue value( QString(p.second.c_str()) );
        //engine_->globalObject().setProperty(name, value);
        //std::cout << p.first << " " << p.second << "\n";
    }

    args << "})";
    const ::std::string argsString = args.str();
    //std::cout << argsString << "\n\n";


    bool canEvaluate = engine_->canEvaluate(argsString.c_str());
    //std::cout << "canEvaluate args " << canEvaluate << "\n";
    if (!canEvaluate)
        BOOST_THROW_EXCEPTION( ::openmedia::script::script::error() );

    QScriptValue argObject = engine_->evaluate(argsString.c_str());
    bool hasUncaughtException = engine_->hasUncaughtException();
    //std::cout << "hasUncaughtException args " << hasUncaughtException << "\n";
    //if (hasUncaughtException)
    //{
    //    std::cout << "uncaughtExceptionLineNumber " << engine_->uncaughtExceptionLineNumber() << "\n";
    //}

    canEvaluate = engine_->canEvaluate(program_.c_str());
    //std::cout << "canEvaluate program " << canEvaluate << "\n";
    if (!canEvaluate)
    {
        //QScriptSyntaxCheckResult syntaxResult = engine_->checkSyntax(program_.c_str());
        //std::cout << "errorLineNumber" << syntaxResult.errorLineNumber() << "\n";
        BOOST_THROW_EXCEPTION( ::openmedia::script::script::error() );
    }

    QScriptValue fun = engine_->evaluate(program_.c_str());
    hasUncaughtException = engine_->hasUncaughtException();
    //std::cout << "hasUncaughtException program " << hasUncaughtException << "\n";
    //if (hasUncaughtException)
    //{
    //    std::cout << "uncaughtExceptionLineNumber " << engine_->uncaughtExceptionLineNumber() << "\n";
    //}

    if (hasUncaughtException)
        BOOST_THROW_EXCEPTION( ::openmedia::script::script::error() );
    
    QScriptValueList funArgs;
    funArgs << argObject;
    QScriptValue res = fun.call(QScriptValue(), funArgs);

    const std::string s = res.toString().toStdString();
    //std::cout << "result: " << s << "\n";
    return s;
}

qt_script::qt_script(const ::std::string & QtScript) 
: script( new qt_script_impl(QtScript) )
{

}


} } }
#endif
