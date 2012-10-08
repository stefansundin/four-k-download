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



// precompiled header begin
#include "DTHeadersMedia.h"
// precompiled header end

#include "DTLameEncoder.h"
#include "DTLameEncoderImpl.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include "DTAudioEncoderImpl.h"
#include "DTCommonPacket.h"
#include "DTCommonCodecExtraData.h"
#include "DTUtils.h"

#include <openmedia/DTError.h>
#include <openmedia/DTAudioFormat.h>
#include <lame/lame.h>

#include <openmedia/DTString.h>
#include "DTMemoryBuffer.h"
#include <openmedia/DTAudioBuffer.h>

#define DT_LAME_CHECK_ERR(ERR) for(;;) { \
    DT_ASSERT( (ERR) >= 0 ); \
    if ((ERR) < 0)    \
        BOOST_THROW_EXCEPTION( errors::invalid_operation() ); \
    break; \
} 

namespace openmedia {

typedef boost::shared_ptr<lame_global_flags> lame_ptr;

namespace details {

class dt_destruct_lame_encoder
{
public:
    void operator () (lame_global_flags * _lame_global_flags)
    {
        lame_close(_lame_global_flags);
    }

};

lame_ptr dt_create_lame_encoder()
{
    lame_global_flags * lame = lame_init();
    DT_ASSERT(NULL != lame);
    return lame_ptr(lame , dt_destruct_lame_encoder() );
}

} // namespace details

class lame_encoder_impl_internal
{
public:
    // move this
    enum StereoMode
    {
      StereoMode_STEREO = 0,
      StereoMode_JOINT_STEREO,
      StereoMode_DUAL_CHANNEL,   /* LAME doesn't supports this! */
      StereoMode_MONO,
      StereoMode_NOT_SET,
      StereoMode_MAX_INDICATOR   /* Don't use this! It's used for sanity checks. */                    
    };

    enum PresetMode 
    {
        /*values from 8 to 320 should be reserved for abr bitrates*/
        /*for abr I'd suggest to directly use the targeted bitrate as a value*/
        PresetMode_ABR_8 = 8,
        PresetMode_ABR_320 = 320,

        PresetMode_V9 = 410, /*Vx to match Lame and VBR_xx to match FhG*/
        PresetMode_VBR_10 = 410,
        PresetMode_V8 = 420,
        PresetMode_VBR_20 = 420,
        PresetMode_V7 = 430,
        PresetMode_VBR_30 = 430,
        PresetMode_V6 = 440,
        PresetMode_VBR_40 = 440,
        PresetMode_V5 = 450,
        PresetMode_VBR_50 = 450,
        PresetMode_V4 = 460,
        PresetMode_VBR_60 = 460,
        PresetMode_V3 = 470,
        PresetMode_VBR_70 = 470,
        PresetMode_V2 = 480,
        PresetMode_VBR_80 = 480,
        PresetMode_V1 = 490,
        PresetMode_VBR_90 = 490,
        PresetMode_V0 = 500,
        PresetMode_VBR_100 = 500,

        /*still there for compatibility*/
        PresetMode_R3MIX = 1000,
        PresetMode_STANDARD = 1001,
        PresetMode_EXTREME = 1002,
        PresetMode_INSANE = 1003,
        PresetMode_STANDARD_FAST = 1004,
        PresetMode_EXTREME_FAST = 1005,
        PresetMode_MEDIUM = 1006,
        PresetMode_MEDIUM_FAST = 1007
    };

    enum VbrMode 
    {
        VbrMode_vbr_off=0,
        VbrMode_vbr_mt,                           // obsolete, same as vbr_mtrh 
        VbrMode_vbr_rh,
        VbrMode_vbr_abr,
        VbrMode_vbr_mtrh,
        VbrMode_vbr_max_indicator,                // Don't use this! It's used for sanity checks.       
        VbrMode_vbr_default = VbrMode_vbr_mtrh    // change this to change the default VBR mode of LAME 
    } ;

public:

    lame_encoder_impl_internal();
    
    void    set_input_sample_rate(int _SampleRate);
    int     get_input_sample_rate() const;

    void    set_channels_count(int _ChannelsCount);
    int     get_channels_count() const;

    void    set_output_sample_rate(int _OutputSampleRate);
    int     get_output_sample_rate() const;

    void    set_scale(float _Scale);
    float   get_scale() const;

    void    set_scale_left(float _Scale);
    float   get_scale_left() const;

    void    set_scale_right(float _Scale);
    float   get_scale_right() const;

    void    set_write_vbr_tag(bool _Val);
    bool    get_write_vbr_tag() const;


  //    internal algorithm selection.  True quality is determined by the bitrate
  //    but this variable will effect quality by selecting expensive or cheap algorithms.
  //    quality=0..9.  0=best (very slow).  9=worst.
  //    recommended:  2     near-best quality, not too slow
  //                  5     good quality, fast
  //                  7     ok quality, really fast

    void    set_quality(int _Quality);
    int     get_quality() const;

    void        set_stereo_mode(StereoMode _Mode);
    StereoMode  get_stereo_mode() const;

    void    set_bit_rate(int _BitRateKbps);
    int     get_bit_rate() const;

    void    set_compression_ratio(float _CompressionRatio);
    float   get_compression_ratio() const;

    void    set_preset( PresetMode _Preset);

    void    set_vbr_mode(VbrMode _Mode);
    VbrMode get_vbr_mode() const;

    // VBR quality level.  0=highest  9=lowest, Range [0,...,10[ 
    void    set_vbr_quality(float _Quality);
    float   get_vbr_quality() const;

    // Ignored except for VBR=vbr_abr (ABR mode) 
    void    set_vbr_mean_bitrate_kbps(int _Bitrate);
    int     get_vbr_mean_bitrate_kbps() const;

    void    set_vbr_min_bitrate_kbps(int _Bitrate);
    int     get_vbr_min_bitrate_kbps() const;

    void    set_vbr_max_bitrate_kbps(int _Bitrate);
    int     get_vbr_max_bitrate_kbps() const;

    void    set_artist(const std::string & Artist);
    void    set_title(const std::string & Title);

    void    set_artist(const std::wstring & Artist);
    void    set_title(const std::wstring & Title);

     
public:
    void set_input_audio_format(const audio_format_ptr _AudioFormat);
    audio_format_ptr get_input_audio_foramt() const;

    void start();
    media_packet_ptr encode(const audio_data * _AudioData);
    media_packet_ptr finish();
    lame_ptr lame()
    {
        return m_Lame;
    }

private:
    lame_ptr m_Lame;
    utils::fixed_memory_buffer m_Buffer;
    uint64_t m_TotalSamplesEncoded;
    int m_OutputSampleRate;
    
};

lame_encoder_impl_internal::lame_encoder_impl_internal() : 
m_Lame(details::dt_create_lame_encoder()),
m_TotalSamplesEncoded(0)
{
    id3tag_add_v2(m_Lame.get());
}

void lame_encoder_impl_internal::set_input_sample_rate(int _SampleRate)
{
    int lameRet = lame_set_in_samplerate(m_Lame.get(), _SampleRate);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_input_sample_rate() const
{
    return lame_get_in_samplerate(m_Lame.get());
}

void lame_encoder_impl_internal::set_channels_count(int _ChannelsCount)
{
    int lameRet = lame_set_num_channels(m_Lame.get(), _ChannelsCount);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_channels_count() const
{
    return lame_get_num_channels(m_Lame.get());
}

void lame_encoder_impl_internal::set_output_sample_rate(int _OutputSampleRate)
{
    int lameRet = lame_set_out_samplerate(m_Lame.get(), _OutputSampleRate);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_output_sample_rate() const
{
    return lame_get_out_samplerate(m_Lame.get());
}

void lame_encoder_impl_internal::set_scale(float _Scale)
{
    int lameRet = lame_set_scale(m_Lame.get(), _Scale);
    DT_LAME_CHECK_ERR(lameRet);
}

float lame_encoder_impl_internal::get_scale() const
{
    return lame_get_scale( m_Lame.get() );
}

void lame_encoder_impl_internal::set_scale_left(float _Scale)
{
    int lameRet = lame_set_scale_left(m_Lame.get(), _Scale);
    DT_LAME_CHECK_ERR(lameRet);
}

float lame_encoder_impl_internal::get_scale_left() const
{
    return lame_get_scale_left( m_Lame.get() );
}

void lame_encoder_impl_internal::set_scale_right(float _Scale)
{
    int lameRet = lame_set_scale_right(m_Lame.get(), _Scale);
    DT_LAME_CHECK_ERR(lameRet);
}

float lame_encoder_impl_internal::get_scale_right() const
{
    return lame_get_scale_right( m_Lame.get() );
}

void lame_encoder_impl_internal::set_write_vbr_tag(bool _Val)
{
    int lameRet = lame_set_bWriteVbrTag(m_Lame.get(), (int)_Val);
    DT_LAME_CHECK_ERR(lameRet);
}

bool lame_encoder_impl_internal::get_write_vbr_tag() const
{
    return ( 0 != lame_get_bWriteVbrTag( m_Lame.get() ) );
}

void lame_encoder_impl_internal::set_quality(int _Quality)
{
    int lameRet = lame_set_quality(m_Lame.get(), _Quality);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_quality() const
{
    return lame_get_quality( m_Lame.get() );
}

void lame_encoder_impl_internal::set_stereo_mode(lame_encoder_impl_internal::StereoMode _Mode)
{
    int lameRet = lame_set_mode(m_Lame.get(), (MPEG_mode)_Mode);
    DT_LAME_CHECK_ERR(lameRet);
}

lame_encoder_impl_internal::StereoMode lame_encoder_impl_internal::get_stereo_mode() const
{
    return static_cast<lame_encoder_impl_internal::StereoMode>( lame_get_mode( m_Lame.get() ) );
}

void lame_encoder_impl_internal::set_bit_rate(int _BitRateKbps)
{
    int lameRet = lame_set_brate(m_Lame.get(), _BitRateKbps);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_bit_rate() const
{
    return lame_get_brate( m_Lame.get() );
}

void lame_encoder_impl_internal::set_compression_ratio(float _CompressionRatio)
{
    int lameRet = lame_set_compression_ratio(m_Lame.get(), _CompressionRatio);
    DT_LAME_CHECK_ERR(lameRet);
}

float lame_encoder_impl_internal::get_compression_ratio() const
{
    return lame_get_compression_ratio( m_Lame.get() );
}

void lame_encoder_impl_internal::set_preset( lame_encoder_impl_internal::PresetMode _Preset)
{
    int lameRet = lame_set_preset( m_Lame.get(), static_cast<int>(_Preset) );
    DT_LAME_CHECK_ERR(lameRet);
}

void lame_encoder_impl_internal::set_vbr_mode(lame_encoder_impl_internal::VbrMode _Mode)
{
    int lameRet = lame_set_VBR(m_Lame.get(), (vbr_mode)_Mode);
    DT_LAME_CHECK_ERR(lameRet);
}

lame_encoder_impl_internal::VbrMode lame_encoder_impl_internal::get_vbr_mode() const
{
    return static_cast<lame_encoder_impl_internal::VbrMode>( lame_get_VBR( m_Lame.get() ) );
}

void lame_encoder_impl_internal::set_vbr_quality(float _Quality)
{
    int lameRet = lame_set_VBR_quality(m_Lame.get(), _Quality);
    DT_LAME_CHECK_ERR(lameRet);
}

float lame_encoder_impl_internal::get_vbr_quality() const
{
    return lame_get_VBR_quality( m_Lame.get() );
}

void lame_encoder_impl_internal::set_vbr_mean_bitrate_kbps(int _Bitrate)
{
    int lameRet = lame_set_VBR_mean_bitrate_kbps(m_Lame.get(), _Bitrate);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_vbr_mean_bitrate_kbps() const
{
    return lame_get_VBR_mean_bitrate_kbps( m_Lame.get() );
}

void lame_encoder_impl_internal::set_vbr_min_bitrate_kbps(int _Bitrate)
{
    int lameRet = lame_set_VBR_min_bitrate_kbps(m_Lame.get(), _Bitrate);
    DT_LAME_CHECK_ERR(lameRet);
}

int lame_encoder_impl_internal::get_vbr_min_bitrate_kbps() const
{
    return lame_get_VBR_min_bitrate_kbps( m_Lame.get() );
}

void lame_encoder_impl_internal::set_vbr_max_bitrate_kbps(int _Bitrate)
{
    int lameRet = lame_set_VBR_max_bitrate_kbps(m_Lame.get(), _Bitrate);
    DT_LAME_CHECK_ERR(lameRet);
}

void lame_encoder_impl_internal::set_artist(const std::string & Artist)
{
    id3tag_set_artist(m_Lame.get(), Artist.c_str());
}

void lame_encoder_impl_internal::set_title(const std::string & Title)
{
    id3tag_set_title(m_Lame.get(), Title.c_str()); 
}

extern "C" int id3tag_set_textinfo_ucs2(lame_global_flags* gfp, char const* id, unsigned short const* text);

namespace {
std::vector<uint16_t> add_bom(const std::wstring & str)
{
    std::vector<uint16_t> tempStr(str.size() + 2);
    tempStr[0] = 0xFEFFu;
    std::copy(str.begin(), str.end(), tempStr.begin() + 1);
    *(tempStr.end() - 1) = 0;

    return tempStr;
}
}

void lame_encoder_impl_internal::set_artist(const std::wstring & Artist)
{
    int lameRet = id3tag_set_textinfo_ucs2(m_Lame.get(), "TPE1", &add_bom(Artist)[0]);
    lameRet;
}

void lame_encoder_impl_internal::set_title(const std::wstring & Title)
{
    int lameRet = id3tag_set_textinfo_ucs2(m_Lame.get(), "TIT2", &add_bom(Title)[0]); 
    lameRet;
}

int lame_encoder_impl_internal::get_vbr_max_bitrate_kbps() const
{
    return lame_get_VBR_max_bitrate_kbps( m_Lame.get() );
}

void lame_encoder_impl_internal::start()
{
    lame_set_write_id3tag_automatic(m_Lame.get(), 0);
    int lameRet = lame_init_params( m_Lame.get() );
    DT_LAME_CHECK_ERR(lameRet);
}

media_packet_ptr lame_encoder_impl_internal::encode(const audio_data * _AudioData)
{
    static const size_t mp3TimeBase = 14112000; // Least common multiple of 44100 and 32000
    const int predSamplesInLame = lame_get_mf_samples_to_encode( m_Lame.get() );
    // from lame:
    // The required mp3buf_size can be computed from num_samples,
    // samplerate and encoding rate, but here is a worst case estimate:
    // mp3buf_size in bytes = 1.25*num_samples + 7200
   
    const size_t samplesCount = (_AudioData) ?  _AudioData->get_samples_count() : 0;
    const size_t bufSize = static_cast<size_t>( 1.25 * samplesCount ) + 7200 ;
    uint8_t * buffer = m_Buffer.get_buffer( bufSize );
    int lameRet = 0;
    
    if (_AudioData)
    {
        const size_t channelsCount = _AudioData->get_channels_count();
        if (channelsCount == 1)
        {
            lameRet = lame_encode_buffer( 
                m_Lame.get(),
                (short*)_AudioData->get_raw_data(),
                (short*)_AudioData->get_raw_data(),
                _AudioData->get_samples_count(),
                buffer,
                bufSize);
        }
        else
        {
            lameRet = lame_encode_buffer_interleaved( 
                m_Lame.get(),
                (short*)_AudioData->get_raw_data(),
                _AudioData->get_samples_count(),
                buffer,
                bufSize);
        }
    }
    else
    {
        lameRet = lame_encode_flush(m_Lame.get(), buffer, bufSize) ;
    }

    const int samplesEncoded = predSamplesInLame +  samplesCount - lame_get_mf_samples_to_encode( m_Lame.get() );
    m_TotalSamplesEncoded += samplesEncoded;

    common_media_packet2_ptr mediaPacket = common_media_packet2_ptr( new common_media_packet2() );
    mediaPacket->setter()->set_data(buffer, lameRet, bufferAllocNew);
    mediaPacket->setter()->set_media_type(DT_AVMEDIA_TYPE_AUDIO);
    mediaPacket->setter()->set_time_base(dt_rational_t(1, mp3TimeBase));
    mediaPacket->setter()->set_duration( samplesEncoded * (mp3TimeBase / get_input_sample_rate()) );

    const dt_ts_t ts = m_TotalSamplesEncoded * (mp3TimeBase / get_input_sample_rate());
    mediaPacket->setter()->set_pts( ts );
    mediaPacket->setter()->set_dts( ts );

    return mediaPacket;
}

media_packet_ptr lame_encoder_impl_internal::finish()
{
    media_packet_ptr endPacket = encode(NULL);
    return endPacket;
}

///////////////////////////////////////////////////////////////////////////////

class audio_encoder_lame_utils::properties
{
public:
    properties() : bitrate_(0) {};
    properties(const std::string & Artist, const std::string & Title, int Bitrate) :
        artist_(Artist),
        title_(Title),
        bitrate_(Bitrate){};
    std::string artist() const { return artist_; }
    std::string title() const { return title_; }
    std::wstring artist_w() const { return utf8_to_utf16(artist_); }
    std::wstring title_w() const { return utf8_to_utf16(title_); }
    int bitrate() const { return bitrate_; }

private:
    std::string artist_;
    std::string title_;
    int bitrate_;

};

boost::shared_ptr<audio_encoder_lame_utils::properties> audio_encoder_lame_utils::create_properties()
{
    return boost::make_shared<audio_encoder_lame_utils::properties>();
}

boost::shared_ptr<audio_encoder_lame_utils::properties> audio_encoder_lame_utils::create_properties(const char * Artist, const char * Title, int Bitrate)
{
    return boost::make_shared<audio_encoder_lame_utils::properties>(Artist, Title, Bitrate);
}

audio_encoder_queue_lame_impl::audio_encoder_queue_lame_impl(const audio_format * _AudioFormat, const audio_encoder_lame_utils::properties * _Properties) 
: m_Lame(NULL), m_AudioBuffer( audio_buffer::create( audio_buffer::audio_buffer_SameFormat, _AudioFormat ) )
, sampleRate_( _AudioFormat ?  _AudioFormat->get_sample_rate() : 0 ), lastData_(false)
{
    std::auto_ptr<lame_encoder_impl_internal> lameEnc = std::auto_ptr<lame_encoder_impl_internal>( new lame_encoder_impl_internal() );

    lameEnc->set_input_sample_rate( _AudioFormat->get_sample_rate() );
    lameEnc->set_channels_count( _AudioFormat->get_channels_count() );
    if (1 == _AudioFormat->get_channels_count())
    {
        lameEnc->set_stereo_mode( lame_encoder_impl_internal::StereoMode_MONO );
    }

    // TODO: default preset
    //
    if (_Properties->bitrate() > 0)
        lameEnc->set_bit_rate(_Properties->bitrate());
    else
    {
        lameEnc->set_preset( lame_encoder_impl_internal::PresetMode_V2);
    }

    lameEnc->set_quality(5);
    
    if (!_Properties->artist_w().empty())
        lameEnc->set_artist(_Properties->artist_w().c_str());

    if (!_Properties->title_w().empty())
        lameEnc->set_title(_Properties->title_w().c_str());
    
        
    lameEnc->start();

    boost::shared_ptr<openmedia::codec_extra_data_common> extraData = boost::make_shared<openmedia::codec_extra_data_common>();

    unsigned char mp3buffer[LAME_MAXMP3BUFFER];
    int imp3 = lame_get_id3v2_tag(lameEnc->lame().get(), mp3buffer, sizeof(mp3buffer));
    DT_ASSERT(imp3 <= LAME_MAXMP3BUFFER);

    extraData->setter()->add_data("ID3V2TAG", mp3buffer, imp3, openmedia::bufferAllocNew);

    m_CodecExtraData = extraData;
    m_Lame = lameEnc.release();
}

codec_extra_data_ptr audio_encoder_queue_lame_impl::get_extra_data() const
{
    return m_CodecExtraData;    
}

void audio_encoder_queue_lame_impl::send_audio(const audio_data * _AudioData)
{
    if (_AudioData)
    {
        m_AudioBuffer->push_back(_AudioData);
        lastData_ = false;
    }
    else
    {
        lastData_ = true;
    }
}

void audio_encoder_queue_lame_impl::send_audio(const audio_data_timed * audioDataTimed)
{
    if (audioDataTimed)
    {
        audio_data_ptr audioData = audioDataTimed->get_media_data();
        send_audio(audioData.get());        
    }
}

media_packet_ptr audio_encoder_queue_lame_impl::receive_packet()
{
    const size_t encodedSamples = sampleRate_;
    if (m_AudioBuffer->get_samples_count() > encodedSamples)
    {
        audio_data_ptr toEncode = m_AudioBuffer->pop_front(encodedSamples);
        media_packet_ptr mediaPacket = m_Lame->encode( toEncode.get() );
        return mediaPacket;
    }
    else if ( lastData_)
    {
        if ( m_AudioBuffer->get_samples_count() )
        {
            audio_data_ptr toEncode = m_AudioBuffer->pop_front(m_AudioBuffer->get_samples_count());
            media_packet_ptr mediaPacket = m_Lame->encode( toEncode.get() );
            return mediaPacket;
        }
        else
        {
            media_packet_ptr mediaPacket = m_Lame->encode( NULL );

            unsigned char mp3buffer[LAME_MAXMP3BUFFER];
            int imp3 = lame_get_id3v1_tag(m_Lame->lame().get(), mp3buffer, sizeof(mp3buffer));
            DT_ASSERT(imp3 <= LAME_MAXMP3BUFFER);
            m_CodecExtraData->setter()->add_data("ID3V1TAG", mp3buffer, imp3, openmedia::bufferAllocNew);

            imp3 = lame_get_lametag_frame(m_Lame->lame().get(), mp3buffer, sizeof(mp3buffer));
            m_CodecExtraData->setter()->add_data("LAMETAG", mp3buffer, imp3, openmedia::bufferAllocNew);
         
            lastData_ = false;
            return mediaPacket;            
        }
    }
    else
        return media_packet_ptr();
}

audio_encoder_queue_lame_impl::~audio_encoder_queue_lame_impl()
{
    delete m_Lame;
}

} // namespace openmedia
