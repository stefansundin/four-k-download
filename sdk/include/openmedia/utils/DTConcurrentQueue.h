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


#ifndef _OPENMEDIA_CONCURRENTQUEUE_H_INCLUDED_
#define _OPENMEDIA_CONCURRENTQUEUE_H_INCLUDED_

#include <queue>
#include <limits>

#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace openmedia {
namespace utils {

template<typename T>
class concurrent_queue : boost::noncopyable
{
    typedef std::queue<T> container;

    class queue_not_empty
    {
    public:
        queue_not_empty(container& queue):
                m_queue(queue)
        {}

        bool operator()() const
        {
            return ! m_queue.empty();
        }
        
        const queue_not_empty & operator = (const queue_not_empty & queue)
        {
            m_queue = queue.m_queue;
            return *this;
        }

    private:
        container& m_queue;
    };


public:

    typedef typename container::value_type         value_type;
    typedef typename container::size_type          size_type;
    typedef typename container::reference          reference;
    typedef typename container::const_reference    const_reference;

    concurrent_queue(size_type capacity = (std::numeric_limits<size_type>::max)())
        : m_capacity(capacity)
    {
    }

    void push(T const& value)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        while( m_queue.size() >= m_capacity )
            m_pushReady.wait(lock);

        m_queue.push(value);
        lock.unlock();
        m_popReady.notify_one();
    }


    template<class Duration>
    bool timed_push(T const& value, Duration const& wait_duration)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        while( m_queue.size() >= m_capacity )
        {
            if (!m_pushReady.timed_wait(lock, wait_duration))
                return false;
        }

        m_queue.push(value);
        lock.unlock();
        m_popReady.notify_one();
        return true;
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(m_mutex);
        return m_queue.empty();
    }

    size_type size() const
    {
        boost::mutex::scoped_lock lock(m_mutex);
        return m_queue.size();
    }

    bool try_pop(T& poppedValue)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        if( m_queue.empty() )
            return false;

        poppedValue = m_queue.front();
        m_queue.pop();
        lock.unlock();
        m_pushReady.notify_one();

        return true;
    }

    void wait_and_pop(T& poppedValue)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        m_popReady.wait(lock, queue_not_empty(m_queue));

        poppedValue = m_queue.front();
        m_queue.pop();
        lock.unlock();
        m_pushReady.notify_one();
    }

    template<class Duration>
    bool timed_wait_and_pop(T& poppedValue, Duration const& wait_duration)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        if( ! m_popReady.timed_wait(lock, wait_duration, queue_not_empty(m_queue)) )
            return false;

        poppedValue = m_queue.front();
        m_queue.pop();
        m_pushReady.notify_one();
        return true;
    }

    void clear()
    {
        boost::mutex::scoped_lock lock(m_mutex);

        container empty;
        std::swap(m_queue, empty);

        lock.unlock();
        m_pushReady.notify_one();
    }

private:
    container m_queue;
    mutable boost::mutex m_mutex;
    boost::condition_variable m_pushReady;
    boost::condition_variable m_popReady;
    size_type m_capacity;
};

}
}

#endif // _OPENMEDIA_CONCURRENTQUEUE_H_INCLUDED_
