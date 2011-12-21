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


/// \file   DTCStdInt.h

#ifndef _DTCSTDINT_H_INCLUDED_
#define _DTCSTDINT_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/cstdint.hpp>

namespace openmedia {

typedef boost::uint8_t uint8_t;
typedef boost::int8_t int8_t;

typedef boost::uint16_t uint16_t;
typedef boost::int16_t int16_t;

typedef boost::uint32_t uint32_t;
typedef boost::int32_t int32_t;

typedef boost::uint64_t uint64_t;
typedef boost::int64_t int64_t;

}

#endif
