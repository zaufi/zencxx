/**
 * \file
 *
 * \brief Class cancel (interface)
 *
 * \date Sun Dec 20 01:45:37 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__CANCEL_HH__
#  define __SYSTEM__THREADS__CANCEL_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/thread_data.hh>

// Standard includes

namespace sys { namespace threads {

inline void cancel()
{
    details::thread_data* thr_data = details::get_specific_thread_data();
    assert("Sanity check" && thr_data);
    mutex::scoped_lock l(&thr_data->m_mut);
    thr_data->m_is_canceled = true;
}

inline bool is_canceled()
{
    details::thread_data* thr_data = details::get_specific_thread_data();
    assert("Sanity check" && thr_data);
    mutex::scoped_lock l(&thr_data->m_mut);
    return thr_data->m_is_canceled;
}

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__CANCEL_HH__
