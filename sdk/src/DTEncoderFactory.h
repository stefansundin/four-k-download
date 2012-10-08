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


/// \file   DTVideoEncoderFactory.h
/// \brief  abstract media encoder factory

#ifndef _DT_OPENMEDIA_ENCODER_FACTORY_H_B5E43987
#define _DT_OPENMEDIA_ENCODER_FACTORY_H_B5E43987

#include <map>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>

namespace openmedia {

/// \class  encoder_factory
template <class Encoder, class EncoderImpl, class SettingsT>
class encoder_factory
{

public:    
    typedef SettingsT Settings;
    typedef boost::function< EncoderImpl * (const boost::shared_ptr<Settings> &) > CreateFuncImpl;
    typedef std::map<std::string, CreateFuncImpl> MapType;
    
    class concrete_encoder : public Encoder
    {
    public:
        concrete_encoder(EncoderImpl * impl): Encoder(impl) {}
    };

    bool register_encoder(const std::string & id, CreateFuncImpl f)
    {
        typename MapType::iterator i = map_.find(id);
        if (i == map_.end())
        {
            map_.insert(typename MapType::value_type(id, f));        
        }
        else
        {
            DT_ASSERT(false);
            return false;
        }
        return true;    
    }

    boost::shared_ptr<Encoder> create(const std::string & id, const boost::shared_ptr<Settings> & settings) const
    {
        typename MapType::const_iterator i = map_.find(id);
        if (i != map_.end())
        {
            EncoderImpl * impl = (i->second)(settings);
            return boost::make_shared<concrete_encoder>(impl);
        }
        else
            return boost::shared_ptr<Encoder>();
    }

private:
    MapType map_; 

public:
    static encoder_factory<Encoder, EncoderImpl, Settings> * get()
    {
        if (!instance_)
        {
            boost::mutex::scoped_lock(lock_);
            if (!instance_)
            {
                encoder_factory * temp = new encoder_factory<Encoder, EncoderImpl, Settings>();
                instance_ = temp;                
            }
        }

        return instance_;    
    }

private:
    encoder_factory(){};
    static encoder_factory<Encoder, EncoderImpl, Settings> * instance_;
    boost::mutex lock_;
};

template <class Encoder, class EncoderImpl, class SettingsT>
encoder_factory<Encoder, EncoderImpl, SettingsT> * encoder_factory<Encoder, EncoderImpl, SettingsT>::instance_ = NULL;

/// \class  register_encoder
template <class T, class Factory>
class register_encoder
{
private:
    template <class L, class Factory1>
    class CreateImpl_
    {
    public:
        static L * create(const boost::shared_ptr<typename Factory1::Settings> & s)
        {
            return new L(s);
        }
    };

public:
    register_encoder(const std::string & id)
    {
        if (class_registered_++ == 0) // Schwartz counter
            Factory::get()->register_encoder(id, &CreateImpl_<T, Factory>::create);            
    };

    ~register_encoder()
    {            
    };
private:
    static int class_registered_;    
};

template<class T, class Factory> int register_encoder<T, Factory>::class_registered_ = 0;

}

#endif
