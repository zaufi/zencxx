/**
 * \file
 *
 * \brief Class simple_fifo_lock (interface)
 *
 * \date Thu Jun 10 14:49:07 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__SIMPLE_FIFO_LOCK_HH__
#  define __SYSTEM__THREADS__SIMPLE_FIFO_LOCK_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/mutex.hh>
#  include <system/threads/details/condition.hh>
#  include <system/threads/exception.hh>

// Standard includes
#  include <cassert>
#  include <list>

namespace sys { namespace threads {

/**
 * \brief Quick replacement for unilock with fifo scheduler
 *
 * The main purpose of this damn class is to serialize access to a mutex.
 *
 */
template <
    typename T = int
  , typename Alloc = std::allocator<T>
  , typename Seq = std::list<T, Alloc>
  >
class simple_fifo_lock
{
    struct locker
    {
        sys::threads::details::mutex* m_mut;
        explicit locker(sys::threads::details::mutex* m) : m_mut(m)
        {
            m_mut->lock();
        }
        ~locker()
        {
            m_mut->unlock();
        }
    };

    mutable sys::threads::details::mutex m_mut;
    mutable sys::threads::details::condition m_cond;

    typedef T request_id_type;
    request_id_type m_current_id;

    typedef Seq requests_queue_type;
    requests_queue_type m_queue;

    bool m_lock_state;

    typedef bool (simple_fifo_lock::*lock_function_type)(int);

    int assign_request_id()
    {
        int rid;
        m_queue.insert(m_queue.end(), rid = m_current_id++);
        return rid;
    }

    void unassign_request_id(request_id_type rid)
    {
        typename requests_queue_type::iterator it = std::find(m_queue.begin(), m_queue.end(), rid);
        assert("sanity check" && it != m_queue.end());
        m_queue.erase(it);
    }

    bool try_lock_impl(int req_id)
    {
        bool result = (!m_lock_state && *m_queue.begin() == req_id);
        if (result)
        {
            assert("sanity check" && !m_lock_state);
            m_lock_state = true;
        }
        return result;
    }

    bool lock_impl(int req_id)
    {
        while (!try_lock_impl(req_id))
            m_cond.wait(m_mut);
        return true;
    }

    bool lock_decorator(lock_function_type lf)
    {
        locker g(&m_mut);
        int request_id;
        try
        {
            request_id = assign_request_id();
        }
        catch (...)
        {
            throw sys::threads::exception::critical_failure(THIS_LOCATION(), ENOMEM);
        }
        // call decorated function
        bool result = (this->*lf)(request_id);
        unassign_request_id(request_id);
        m_cond.broadcast();
        return result;
    }

public:
    simple_fifo_lock() : m_current_id(request_id_type()), m_lock_state(false)
    {
        int rc = m_mut.initialize();
        if (rc) throw sys::threads::exception::critical_failure(THIS_LOCATION(), rc);
        rc = m_cond.initialize();
        if (rc) throw sys::threads::exception::critical_failure(THIS_LOCATION(), rc);
    }

    ~simple_fifo_lock()
    {
        int rc = m_cond.destroy();
        assert("Lock destroy failed" && 0 == rc);
        rc = m_mut.destroy();
        assert("Lock destroy failed" && 0 == rc);
    }

    bool try_lock()
    {
        return lock_decorator(&simple_fifo_lock::try_lock_impl);
    }

    void lock()
    {
        lock_decorator(&simple_fifo_lock::lock_impl);
    }

    void unlock()
    {
        locker l(&m_mut);
        m_lock_state = false;
        m_cond.broadcast();
    }

    class scoped_lock
    {
        simple_fifo_lock* m_lock;

    public:
        scoped_lock(simple_fifo_lock& l) : m_lock(&l)
        {
            m_lock->lock();
        }
        ~scoped_lock()
        {
            m_lock->unlock();
        }
    };
};

typedef simple_fifo_lock<> simple_ordered_lock;

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__SIMPLE_FIFO_LOCK_HH__
