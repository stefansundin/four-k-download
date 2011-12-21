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


/// \file   DTDeclareImpl.h
#ifndef _DTDECLAREIMPL_H_INCLUDED_
#define _DTDECLAREIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTAssert.h>

#define DT_DECLARE_IMPL2(CLASS_NAME)    public: \
class Impl; \
                                        protected:\
                                        const CLASS_NAME::Impl * impl() const {DT_STRONG_ASSERT(NULL != m_pImpl); return m_pImpl;}\
                                        CLASS_NAME::Impl * impl()       {DT_STRONG_ASSERT(NULL != m_pImpl); return m_pImpl;}

#define DT_DECLARE_IMPL3(CLASS_NAME)    DT_DECLARE_IMPL2(CLASS_NAME)\
                                        private:\
                                        Impl * m_pImpl;\
                                        protected:\
                                        CLASS_NAME(Impl * _Impl);



#endif
