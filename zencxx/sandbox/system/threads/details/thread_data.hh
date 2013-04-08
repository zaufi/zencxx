/**
 * \file
 *
 * \brief Class thread_data (interface)
 *
 * \date Mon Nov  9 03:07:44 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DETAILS__THREAD_DATA_HH__
#  define __SYSTEM__THREADS__DETAILS__THREAD_DATA_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/mutex.hh>
#  include <system/threads/specific.hh>
#  include <system/threads/details/condition.hh>

// Standard includes

namespace sys { namespace threads { namespace details {

struct thread_data
{
    mutable ::sys::threads::mutex m_mut;
    bool m_is_canceled;                                     /// \todo Is \c sigatomic_t would be better here?
    condition* m_cur_cond;

    thread_data() : m_is_canceled(false), m_cur_cond(0)
    {
    }
};

specific_ptr<thread_data> g_thread_data __attribute__((weak));

inline thread_data* get_specific_thread_data()
{
    thread_data* td = g_thread_data.get();
    return td ? td : (g_thread_data = new thread_data());
}

}}}                                                         // namespace details, threads, sys
#endif                                                      // __SYSTEM__THREADS__DETAILS__THREAD_DATA_HH__
