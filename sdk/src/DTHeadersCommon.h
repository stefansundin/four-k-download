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


#ifndef _DT_HEADERS_COMMON_H_
#define _DT_HEADERS_COMMON_H_

#define BOOST_THREAD_USE_LIB

#ifdef _MSC_VER
#pragma once


#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <numeric>

#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/regex.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/program_options/detail/convert.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#endif
#endif 
