/**
 * \file
 *
 * \brief Class cancelation checker (interface)
 *
 * \date Mon Nov  9 03:07:44 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__CANCELATION_CHECKER_HH__
#  define __SYSTEM__THREADS__DETAILS__CANCELATION_CHECKER_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/exception.hh>
#  include <system/threads/details/thread_data.hh>

// Standard includes

namespace sys { namespace threads { namespace details {

typedef thread_data* thread_data_descriptor;

class cancelation_checker
{
    typedef ::sys::threads::mutex::scoped_lock scoped_lock;
    thread_data_descriptor m_thr_data;

    void cancelation_check() const                          // throw (thread_canceled)
    {
        if (m_thr_data->m_is_canceled)
            throw threads::exception::thread_canceled();
    }

public:
    explicit cancelation_checker(condition* cond)
      : m_thr_data(get_specific_thread_data())
    {
        if (!m_thr_data) return;
        scoped_lock g(&m_thr_data->m_mut);
        cancelation_check();
        m_thr_data->m_cur_cond = cond;
    }

    ~cancelation_checker()                                  // throw (thread_canceled)
    {
        if (!m_thr_data) return;
        scoped_lock g(&m_thr_data->m_mut);
        // Exception into destructor looks very strange, I know. But this service
        // class is orient for using withing pthread api, without any exceptions.
        // So we can be nonconcerned about exception masking.
        /// \warning NO exception in same scope with this class objects!
        cancelation_check();
        m_thr_data->m_cur_cond = 0;
    }
};


}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__CANCELATION_CHECKER_HH__
