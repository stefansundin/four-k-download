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
#include "DTHeadersDownload.h"
// precompiled header end

#include <deque>
#include <iostream>

#define BOOST_THREAD_USE_LIB
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "DTHttpDownloaderImpl.h"

namespace openmedia { namespace downloader {

/// \class http_downloader_queue
class http_downloader_queue
{
public:
    static http_downloader_queue * get()
    {
        if (!instance_)
        {
            boost::mutex::scoped_lock lock(create_lock_);
            if (!instance_)
            {
                http_downloader_queue * temp = new http_downloader_queue();
                instance_ = temp;                
            }
        }

        return instance_;    
    }

private:

    void DoDownloadStateNotify(http_downloader::DownloadStateNotify downloadStateNotifyExternal, http_downloader::download_state_t State)
    {
        //std::cerr << "http_downloader_queue::DoDownloadStateNotify: "<< State << "\n";
        switch (State)
        {
        case http_downloader::stateCancel:
        case http_downloader::stateFinish:
        case http_downloader::stateError:
            OnFinishDownload();
            break;
        case http_downloader::statePause:
            {
                boost::mutex::scoped_lock lock(oper_lock_);
                activeDownloaders_--;
                CheckQueue();
            }
            break;
        case http_downloader::stateResume:
            {
                boost::mutex::scoped_lock lock(oper_lock_);
                activeDownloaders_++;
            }
            break;
        default:
            break;
        }

        if (downloadStateNotifyExternal)
            downloadStateNotifyExternal(State);
        //std::cerr << "http_downloader_queue::DoDownloadStateNotify finish\n";
    }

    void OnFinishDownload()
    {
        //std::cerr << "http_downloader_queue::OnFinishDownload\n";
        boost::mutex::scoped_lock lock(oper_lock_);
        //std::cerr << "http_downloader_queue::OnFinishDownload: enter\n";
        activeDownloaders_--;
        //std::cerr << "http_downloader_queue::CheckQueue: activeDownloaders_: " << activeDownloaders_ << "\n";
        CheckQueue();
        //std::cerr << "http_downloader_queue::OnFinishDownload finish\n";
    }

    struct CreateInstanceDesc
    {
        size_t id_;
        http_downloader::Impl::CreateInstaceFunc createInstaceFunc_;
        boost::function< void(http_downloader_instance *) > setInstanceFunc_;
        http_downloader::DownloadStateNotify downloadStateNotify_;
    };

    void OnCancelElement(size_t Id)
    {
        //std::cerr << "http_downloader_queue::OnCancelElement\n";
        boost::mutex::scoped_lock lock(oper_lock_);
        //std::cerr << "http_downloader_queue::OnCancelElement: enter\n";

        for (::std::deque< CreateInstanceDesc >::iterator i = queue_.begin(); i != queue_.end(); ++i)
        {
            if (i->id_ == Id)
            {
                std::cerr << "http_downloader_queue::OnCancelElement: found: " << Id << "\n";
                queue_.erase(i);
                return;
            }
        }
        //std::cerr << "http_downloader_queue::OnCancelElement finish\n";
    }

    void CheckQueue()
    {
        //std::cerr << "http_downloader_queue::CheckQueue\n";
        if (activeDownloaders_ < DownloadersCount && !queue_.empty() && !shutdown_)
        {
            //std::cerr << "http_downloader_queue::CheckQueue: create instance\n";
            CreateInstanceDesc desc = queue_.front();
            if (desc.createInstaceFunc_)
            {
                std::auto_ptr<http_downloader_instance> inst = desc.createInstaceFunc_( desc.downloadStateNotify_ );

                if (desc.setInstanceFunc_)
                    desc.setInstanceFunc_(inst.release());
            }
            queue_.pop_front();
            activeDownloaders_++;
            //std::cerr << "http_downloader_queue::CheckQueue: activeDownloaders_: " << activeDownloaders_ << "\n";
        }
        //std::cerr << "http_downloader_queue::CheckQueue finish\n";
    }


public:
    http_downloader_queue() : activeDownloaders_(0), newId_(0), shutdown_(false)
    {}

    http_downloader::Impl::OnCancelElement add(  
        http_downloader::Impl::CreateInstaceFunc CreateInst,
        boost::function< void(http_downloader_instance *) > SetInst,
        http_downloader::DownloadStateNotify onChangeState
        )
    {
        //std::cerr << "http_downloader_queue::add\n";
        boost::mutex::scoped_lock lock(oper_lock_);
        //std::cerr << "http_downloader_queue::add: enter\n";

        CreateInstanceDesc desc = {
            ++newId_,
            CreateInst,
            SetInst,
            boost::bind( &http_downloader_queue::DoDownloadStateNotify, this, onChangeState, _1 )
        };

        queue_.push_back(desc);

        CheckQueue();
        //std::cerr << "http_downloader_queue::add finish\n";
        return boost::bind( &http_downloader_queue::OnCancelElement, this, newId_ );                                    
    }

    boost::mutex & lock()
    {
        return oper_lock_;
    }

    void shutdown()
    {
        shutdown_ = true;    
    }

private:

    ::std::deque< CreateInstanceDesc > queue_;

    static const size_t DownloadersCount = 3;
    size_t activeDownloaders_;
    size_t newId_;
    boost::mutex oper_lock_;
    bool shutdown_;

    static boost::mutex create_lock_;
    static http_downloader_queue * instance_;
};

http_downloader_queue * http_downloader_queue::instance_;
boost::mutex http_downloader_queue::create_lock_;

http_downloader::command_result_t http_downloader::Impl::pause()
{
    return instance_ ? instance_->pause() : resultOk;
}

http_downloader::command_result_t http_downloader::Impl::resume()
{
    return instance_ ? instance_->resume() : resultOk;
}

http_downloader::command_result_t http_downloader::Impl::cancel()
{
    if (instance_)
        return instance_->cancel();
    else
    {
        return resultOk;
    }
}

http_downloader::Impl::Impl( http_downloader_instance * Instance) : instance_(Instance)
{    
}

http_downloader::Impl::Impl(CreateInstaceFunc createInstanceFunc,
                            http_downloader::DownloadStateNotify onChangeState) : instance_(NULL), downloadStateNotify_(onChangeState)
{
    //instance_ = createInstanceFunc( boost::bind(&http_downloader::Impl::DoDownloadStateNotify, this, _1) ).release();          
    //std::cerr << "http_downloader::Impl::Impl\n";
    onCancelElement_ = http_downloader_queue::get()->add(
        createInstanceFunc, 
        boost::bind( &http_downloader::Impl::set_instance, this, _1), 
        onChangeState);
    //std::cerr << "http_downloader::Impl::Impl finish\n";
}

void http_downloader::Impl::DoDownloadStateNotify(http_downloader::download_state_t State)
{
    //std::cerr << "http_downloader::Impl::DoDownloadStateNotify(" << State << "\n";
    if (downloadStateNotify_)
        downloadStateNotify_(State);
}

http_downloader::Impl::~Impl()
{
    //std::cerr << "http_downloader::Impl::~Impl\n";
    if (!instance_)
    {
        boost::mutex::scoped_lock(http_downloader_queue::get()->lock());
        if (!instance_)
        {
            if (onCancelElement_)
            {
                //std::cerr << "do onCancelElement\n";
                onCancelElement_();
                //std::cerr << "http_downloader::Impl::~Impl finish\n";
                return;
            }
        }
    }

    //std::cerr << "delete instance_\n";
    delete instance_;

    //std::cerr << "http_downloader::Impl::~Impl finish\n";
}

void http_downloader::Impl::set_instance(http_downloader_instance * Instance)
{
    //std::cerr << "http_downloader::Impl::set_instance\n";
    instance_ = Instance;    
}

void http_downloader::Impl::shutdown()
{
    http_downloader_queue::get()->shutdown();    
}

}
}
