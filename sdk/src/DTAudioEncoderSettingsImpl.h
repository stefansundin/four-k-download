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


/// \file   DTAudioEncoderSettingsImpl.h

#ifndef _DTAUDIOENCODERSETTINGSIMPL_H_INCLUDED_
#define _DTAUDIOENCODERSETTINGSIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTAudioEncoder.h>

namespace openmedia {

class audio_encoder_settings::Impl
{
public:
    Impl(EncoderMode Mode, int BitRate, double Quality) : mode_(Mode), bit_rate_(BitRate), quality_(Quality)
    {
    }

    EncoderMode     get_mode() const { return mode_; }
    int             get_bit_rate() const { return bit_rate_; }
    double          get_quality() const { return quality_; }

private:
    EncoderMode mode_;
    int bit_rate_;
    double quality_;
    
};

}

#endif // #ifndef _DTAUDIOENCODERSETTINGSIMPL_H_INCLUDED_
