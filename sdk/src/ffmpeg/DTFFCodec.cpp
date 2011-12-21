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


#include "DTFFCodec.h"

#include <openmedia/DTError.h>

#define DT_FF_NEED_AVCODEC
#include "DTFFHeader.h"

#include <openmedia/DTAdditionalSettings.h>
#include <openmedia/DTCodec.h>
#include <openmedia/DTDecoder.h>

namespace openmedia {

void AVCodecContextAdditionalSettings(AVCodecContext * _CodecContext, const additional_settings * _AdditionalSettings)
{
    const size_t settingsCount = _AdditionalSettings->size();
    for (size_t i = 0; i < settingsCount; ++i)
    {
        std::string _Key;
        std::string _Value;
        _AdditionalSettings->get_at(i, _Key, _Value);
        
        if (settings::codec::DT_FF_DEBUG == _Key)
        {
#define _SET_FF_DEBUG_VALUE_(DT, FF) if (settings::codec::DT == _Value) _CodecContext->debug |= FF

            _SET_FF_DEBUG_VALUE_(       DT_FF_DEBUG_PICT,         FF_DEBUG_PICT_INFO);
            //else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_RC,           FF_DEBUG_RC); // encoding
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_BITSTREAM,    FF_DEBUG_BITSTREAM);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_MB_TYPE,      FF_DEBUG_MB_TYPE);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_QP,           FF_DEBUG_QP);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_MV,           FF_DEBUG_MV);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_DCT_COEFF,    FF_DEBUG_DCT_COEFF);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_SKIP,         FF_DEBUG_SKIP);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_STARTCODE,    FF_DEBUG_STARTCODE);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_PTS,          FF_DEBUG_PTS);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_ER,           FF_DEBUG_ER);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_MMCO,         FF_DEBUG_MMCO);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_BUGS,         FF_DEBUG_BUGS);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_VIS_QP,       FF_DEBUG_VIS_QP);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_VIS_MB_TYPE,  FF_DEBUG_VIS_MB_TYPE);
            else _SET_FF_DEBUG_VALUE_(  DT_FF_DEBUG_BUFFERS,      FF_DEBUG_BUFFERS);

#undef _SET_FF_DEBUG_VALUE_

        }
        else if (settings::codec::DT_FF_ER == _Key)
        {

#define _SET_FF_ER_VALUE_(DT, FF) if (settings::codec::DT == _Value) _CodecContext->error_recognition = FF

            _SET_FF_ER_VALUE_(      DT_FF_ER_CAREFUL,           FF_ER_CAREFUL);
            else _SET_FF_ER_VALUE_( DT_FF_ER_COMPLIANT,         FF_ER_COMPLIANT);
            else _SET_FF_ER_VALUE_( DT_FF_ER_AGGRESSIVE,        FF_ER_AGGRESSIVE);
            else _SET_FF_ER_VALUE_( DT_FF_ER_VERY_AGGRESSIVE,   FF_ER_VERY_AGGRESSIVE);

#undef _SET_FF_ER_VALUE_
 
        }
        else if (settings::codec::DT_FF_FLAG == _Key)
        {
#define _SET_FF_FLAG_VALUE_(DT, FF) if (settings::codec::DT == _Value) _CodecContext->flags |= FF

            _SET_FF_FLAG_VALUE_(         DT_FF_FLAG_MV4           , CODEC_FLAG_4MV                            );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_OBMC          , CODEC_FLAG_OBMC                           );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_QPEL          , CODEC_FLAG_QPEL                           );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_LOOP          , CODEC_FLAG_LOOP_FILTER                    );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_GMC           , CODEC_FLAG_GMC                            );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_MV0           , CODEC_FLAG_MV0                            );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_PART          , CODEC_FLAG_PART                           );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_GRAY          , CODEC_FLAG_GRAY                           );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_PSNR          , CODEC_FLAG_PSNR                           );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_NAQ           , CODEC_FLAG_NORMALIZE_AQP                  );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_ILDCT         , CODEC_FLAG_INTERLACED_DCT                 );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_LOW_DELAY     , CODEC_FLAG_LOW_DELAY                      );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_ALT           , CODEC_FLAG_ALT_SCAN                       );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_GLOBAL_HEADER , CODEC_FLAG_GLOBAL_HEADER                  );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_BITEXACT      , CODEC_FLAG_BITEXACT                       );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_AIC           , CODEC_FLAG_AC_PRED                        );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_UMV           , CODEC_FLAG_H263P_UMV                      );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_CBP           , CODEC_FLAG_CBP_RD                         );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_QPRD          , CODEC_FLAG_QP_RD                          );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_AIV           , CODEC_FLAG_H263P_AIV                      );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_SLICE         , CODEC_FLAG_H263P_SLICE_STRUCT             );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_ILME          , CODEC_FLAG_INTERLACED_ME                  );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_SCAN_OFFSET   , CODEC_FLAG_SVCD_SCAN_OFFSET               );
            else _SET_FF_FLAG_VALUE_(    DT_FF_FLAG_CGOP          , CODEC_FLAG_CLOSED_GOP                     );

#undef _SET_FF_FLAG_VALUE_

        } else if (settings::codec::DT_FF_BUG == _Key)
        {

#define _SET_FF_BUG_VALUE_(DT, FF) if (settings::codec::DT == _Value) _CodecContext->workaround_bugs |= FF

            _SET_FF_BUG_VALUE_(         DT_FF_BUG_AUTODETECT,       FF_BUG_AUTODETECT);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_OLD_MSMPEG4,      FF_BUG_OLD_MSMPEG4);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_XVID_ILACE,       FF_BUG_XVID_ILACE);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_UMP4,             FF_BUG_UMP4);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_NO_PADDING,       FF_BUG_NO_PADDING);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_AMV,              FF_BUG_AMV);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_AC_VLC,           FF_BUG_AC_VLC);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_QPEL_CHROMA,      FF_BUG_QPEL_CHROMA);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_STD_QPEL,         FF_BUG_STD_QPEL);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_QPEL_CHROMA2,     FF_BUG_QPEL_CHROMA2);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_DIRECT_BLOCKSIZE, FF_BUG_DIRECT_BLOCKSIZE);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_EDGE,             FF_BUG_EDGE);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_HPEL_CHROMA,      FF_BUG_HPEL_CHROMA);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_DC_CLIP,          FF_BUG_DC_CLIP);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_MS,               FF_BUG_MS);
            else _SET_FF_BUG_VALUE_(    DT_FF_BUG_TRUNC,            FF_BUG_TRUNCATED);  

#undef _SET_FF_BUG_VALUE_
        }
    }
}

namespace details {

class _AVCodecContextDestroy
{
public:
    void operator()(AVCodecContext * _CodecContext)
    {  
        if (NULL != _CodecContext)
        {
            if (_CodecContext->codec)
            {
                avcodec_close(_CodecContext);
            }

            av_freep(&_CodecContext->extradata);
            av_freep(&_CodecContext->inter_matrix);
            av_freep(&_CodecContext->intra_matrix);
            av_freep(&_CodecContext->rc_eq);
            av_freep(&_CodecContext->rc_override);

            avcodec_default_free_buffers(_CodecContext);
            av_free(_CodecContext);
        }
    }
};

} // namespace details

AVCodecContextPtr dt_create_codec_context()
{
    AVCodecContextPtr codecContext(avcodec_alloc_context(), details::_AVCodecContextDestroy());
    return codecContext;
}

AVCodecContextPtr dt_create_codec_context(AVCodecContext * _AVCodecContext)
{
    return AVCodecContextPtr(_AVCodecContext, details::_AVCodecContextDestroy());
}


void dt_open_ff_decoder(AVCodecContext * _AVCodecContext, dt_codec_id_t _CodecID)
{
    AVCodec * avDecoder = avcodec_find_decoder(DT2FFType(_CodecID));
    if (!avDecoder)
    {
        BOOST_THROW_EXCEPTION(decoder::create_error() << boost::errinfo_api_function("avcodec_find_decoder"));
    }

    int dt_err = avcodec_open(_AVCodecContext, avDecoder);
    FF_CHECK_ERR(dt_err);
    if (FF_ERR(dt_err))
		DT_THROW_AVERROR(decoder::create_error(), dt_err, "avcodec_open");
}

bool get_ff_codec_name_by_id(dt_codec_id_t _CodecId, std::string & _Name, std::string & _LongName)
{
    AVCodec * codec = NULL;
    while ((codec = av_codec_next(codec)))
    {
        if (codec->id == DT2FFType(_CodecId))
        {
            _Name = codec->name;
            _LongName = codec->long_name;
            return true;
        }
    }
    return false;    
}

}

