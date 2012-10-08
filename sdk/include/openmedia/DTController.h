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


#ifndef _DT_CONTROLLER_H_INCLUDED_
#define _DT_CONTROLLER_H_INCLUDED_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace openmedia {

class Controller : boost::noncopyable
{
public:
    Controller();
    void cancel();
public:
    class Impl;
    Controller(boost::shared_ptr<Impl> impl);
private:
    boost::shared_ptr<Impl> impl_;

};

typedef boost::shared_ptr<Controller> ControllerPtr;
typedef boost::shared_ptr<Controller::Impl> ControllerImplPtr;

}

#endif
