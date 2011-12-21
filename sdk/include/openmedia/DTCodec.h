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


/// \file   DTCodec.h

#ifndef _DTCODEC_H_INCLUDED_
#define _DTCODEC_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTAdditionalSettings.h>

namespace openmedia {

namespace settings {
namespace codec {

// -er      <int>           .DVA. set error detection aggressivity
DECLARE_DT_SETTING(DT_FF_ER);

// careful                  .DV..
DECLARE_DT_SETTING(DT_FF_ER_CAREFUL);
// compliant                .DV..
DECLARE_DT_SETTING(DT_FF_ER_COMPLIANT);
// aggressive               .DV..
DECLARE_DT_SETTING(DT_FF_ER_AGGRESSIVE);
// very_aggressive          .DV..
DECLARE_DT_SETTING(DT_FF_ER_VERY_AGGRESSIVE);

/////////////////////////////////////////////////////////////////////////////////////////

//-flags          <flags> EDVA.
DECLARE_DT_SETTING(DT_FF_FLAG);

//mv4                     E.V.. use four motion vector by macroblock (mpeg4)
DECLARE_DT_SETTING(DT_FF_FLAG_MV4);
//obmc                    E.V.. use overlapped block motion compensation (h263+)
DECLARE_DT_SETTING(DT_FF_FLAG_OBMC);
//qpel                    E.V.. use 1/4 pel motion compensation
DECLARE_DT_SETTING(DT_FF_FLAG_QPEL);
//loop                    E.V.. use loop filter
DECLARE_DT_SETTING(DT_FF_FLAG_LOOP);
//gmc                     E.V.. use gmc
DECLARE_DT_SETTING(DT_FF_FLAG_GMC);
//mv0                     E.V.. always try a mb with mv=<0,0>
DECLARE_DT_SETTING(DT_FF_FLAG_MV0);
//part                    E.V.. use data partitioning
DECLARE_DT_SETTING(DT_FF_FLAG_PART);
//gray                    EDV.. only decode/encode grayscale
DECLARE_DT_SETTING(DT_FF_FLAG_GRAY);
//psnr                    E.V.. error[?] variables will be set during encoding
DECLARE_DT_SETTING(DT_FF_FLAG_PSNR);
//naq                     E.V.. normalize adaptive quantization
DECLARE_DT_SETTING(DT_FF_FLAG_NAQ);
//ildct                   E.V.. use interlaced dct
DECLARE_DT_SETTING(DT_FF_FLAG_ILDCT);
//low_delay               EDV.. force low delay
DECLARE_DT_SETTING(DT_FF_FLAG_LOW_DELAY);
//alt                     E.V.. enable alternate scantable (mpeg2/mpeg4)
DECLARE_DT_SETTING(DT_FF_FLAG_ALT);
//global_header           E.VA. place global headers in extradata instead of every keyframe
DECLARE_DT_SETTING(DT_FF_FLAG_GLOBAL_HEADER);
//bitexact                EDVAS use only bitexact stuff (except (i)dct)
DECLARE_DT_SETTING(DT_FF_FLAG_BITEXACT);
//aic                     E.V.. h263 advanced intra coding / mpeg4 ac prediction
DECLARE_DT_SETTING(DT_FF_FLAG_AIC);
//umv                     E.V.. use unlimited motion vectors
DECLARE_DT_SETTING(DT_FF_FLAG_UMV);
//cbp                     E.V.. use rate distortion optimization for cbp
DECLARE_DT_SETTING(DT_FF_FLAG_CBP);
//qprd                    E.V.. use rate distortion optimization for qp selection
DECLARE_DT_SETTING(DT_FF_FLAG_QPRD);
//aiv                     E.V.. h263 alternative inter vlc
DECLARE_DT_SETTING(DT_FF_FLAG_AIV);
//slice                   E.V..
DECLARE_DT_SETTING(DT_FF_FLAG_SLICE);
//ilme                    E.V.. interlaced motion estimation
DECLARE_DT_SETTING(DT_FF_FLAG_ILME);
//scan_offset             E.V.. will reserve space for svcd scan offset user data
DECLARE_DT_SETTING(DT_FF_FLAG_SCAN_OFFSET);
//cgop                    E.V.. closed gop
DECLARE_DT_SETTING(DT_FF_FLAG_CGOP);


/////////////////////////////////////////////////////////////////////////////////////////

// -debug   <flags> EDVAS   print specific debug info
DECLARE_DT_SETTING(DT_FF_DEBUG);

// pict                     .DV.. picture info
DECLARE_DT_SETTING(DT_FF_DEBUG_PICT);
// rc                       E.V.. rate control
DECLARE_DT_SETTING(DT_FF_DEBUG_RC);
// bitstream                .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_BITSTREAM);
// mb_type                  .DV.. macroblock (MB) type
DECLARE_DT_SETTING(DT_FF_DEBUG_MB_TYPE);
// qp                       .DV.. per-block quantization parameter (QP)
DECLARE_DT_SETTING(DT_FF_DEBUG_QP);
// mv                       .DV.. motion vector
DECLARE_DT_SETTING(DT_FF_DEBUG_MV);
// dct_coeff                .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_DCT_COEFF);
// skip                     .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_SKIP);
// startcode                .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_STARTCODE);
// pts                      .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_PTS);
// er                       .DV..   error recognition
DECLARE_DT_SETTING(DT_FF_DEBUG_ER);
// mmco                     .DV..   memory management control operations (H.264)
DECLARE_DT_SETTING(DT_FF_DEBUG_MMCO);
// bugs                     .DV..
DECLARE_DT_SETTING(DT_FF_DEBUG_BUGS);
// vis_qp                   .DV..   visualize quantization parameter (QP), lower QP are tinted greener
DECLARE_DT_SETTING(DT_FF_DEBUG_VIS_QP);
// vis_mb_type              .DV.. visualize block types
DECLARE_DT_SETTING(DT_FF_DEBUG_VIS_MB_TYPE);
// buffers                  .DV.. picture buffer allocations
DECLARE_DT_SETTING(DT_FF_DEBUG_BUFFERS);

/////////////////////////////////////////////////////////////////////////////////////////

// -bug                 <flags> .DV.. workaround not auto detected encoder bugs
DECLARE_DT_SETTING(DT_FF_BUG);

//      autodetect              .DV..
DECLARE_DT_SETTING(DT_FF_BUG_AUTODETECT);
//      old_msmpeg4             .DV.. some old lavc generated msmpeg4v3 files (no autodetection)
DECLARE_DT_SETTING(DT_FF_BUG_OLD_MSMPEG4);
//      xvid_ilace              .DV.. Xvid interlacing bug (autodetected if fourcc==XVIX)
DECLARE_DT_SETTING(DT_FF_BUG_XVID_ILACE);
//      ump4                    .DV.. (autodetected if fourcc==UMP4)
DECLARE_DT_SETTING(DT_FF_BUG_UMP4);
//      no_padding              .DV.. padding bug (autodetected)
DECLARE_DT_SETTING(DT_FF_BUG_NO_PADDING);
//      amv                     .DV..
DECLARE_DT_SETTING(DT_FF_BUG_AMV);
//      ac_vlc                  .DV.. illegal vlc bug (autodetected per fourcc)
DECLARE_DT_SETTING(DT_FF_BUG_AC_VLC);
//      qpel_chroma             .DV..
DECLARE_DT_SETTING(DT_FF_BUG_QPEL_CHROMA);
//      std_qpel                .DV.. old standard qpel (autodetected per fourcc/version)
DECLARE_DT_SETTING(DT_FF_BUG_STD_QPEL);
//      qpel_chroma2            .DV..
DECLARE_DT_SETTING(DT_FF_BUG_QPEL_CHROMA2);
//      direct_blocksize         .DV.. direct-qpel-blocksize bug (autodetected per fourcc/version)
DECLARE_DT_SETTING(DT_FF_BUG_DIRECT_BLOCKSIZE);
//      edge                    .DV.. edge padding bug (autodetected per fourcc/version)
DECLARE_DT_SETTING(DT_FF_BUG_EDGE);
//      hpel_chroma             .DV..
DECLARE_DT_SETTING(DT_FF_BUG_HPEL_CHROMA);
//      dc_clip                 .DV..
DECLARE_DT_SETTING(DT_FF_BUG_DC_CLIP);
//      ms                      .DV.. workaround various bugs in microsofts broken decoders
DECLARE_DT_SETTING(DT_FF_BUG_MS);
//      trunc                   .DV.. trancated frames
DECLARE_DT_SETTING(DT_FF_BUG_TRUNC);

} // namespace settings
} // namespace codec

} // namespace openmedia

#endif // #ifndef _DTCODEC_H_INCLUDED_
