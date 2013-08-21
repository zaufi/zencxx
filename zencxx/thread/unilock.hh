/**
 * \file
 *
 * \brief Class \c zencxx::unilock (interface)
 *
 * \date Wed Jul 17 11:46:50 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2013 Alex Turbov and contributors, all rights reserved.
 * This is free software. It is licensed for use, modification and
 * redistribution under the terms of the GNU Lesser General Public License,
 * version 3 or later <http://gnu.org/licenses/lgpl.html>
 *
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZenCxx is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef __ZENCXX__THREAD__UNILOCK_HH__
# define __ZENCXX__THREAD__UNILOCK_HH__

// Project specific includes
# include <zencxx/thread/default_scheduler.hh>
# include <zencxx/thread/priority_scheduler.hh>
# include <zencxx/details/export.hh>

// Standard includes
# include <boost/thread/condition_variable.hpp>
# include <boost/thread/mutex.hpp>
# include <utility>

namespace zencxx { inline namespace thread {
/**
 * \brief A matrix specification for exclusive lock
 */
ZENCXX_EXPORT struct exclusive_lock
{
    enum type
    {
        lock
    };
    static constexpr bool matrix[1][1] = {{false}};
    static constexpr type default_lock = lock;
};

/**
 * \brief A matrix specification for read/write lock
 *
 * Allows multiple read locks.
 */
ZENCXX_EXPORT struct rw_lock
{
    enum type
    {
        read
      , write
    };
    static constexpr bool matrix[2][2] = {                  //        Read  Write
        {true,  false}                                      // Read    1      0
      , {false, false}                                      // Write   0      0
    };
};

/**
 * \brief [Type brief class description here]
 *
 * [More detailed description here]
 *
 */
template <typename Scheduler>
class unilock
{
    typedef Scheduler scheduler_type;

    template <typename... Args>
    using lock_func_t = bool (unilock::*)(boost::mutex::scoped_lock&, int, Args&&...);

public:
    template <typename... Args>
    void lock(Args&&... args)
    {
        lock_decorator(&unilock::lock_impl, std::forward<Args>(args)...);
    }

    template <typename... Args>
    bool try_lock(Args&&... args)
    {
        return lock_decorator(&unilock::try_lock_impl, std::forward<Args>(args)...);
    }

    /// \todo Is thread interrupt handler needed? Really?
    template <typename... Args>
    void unlock(Args&&... args)
    {
        try
        {
            boost::mutex::scoped_lock l(m_mut);             // may throw boost::thread_resource_error
            m_sched.unlock(std::forward<Args>(args)...);
        }
        catch (...)
        {
            throw boost::thread_resource_error();
        }
        m_cond.notify_all();
    }

private:
    template <typename... Args>
    bool lock_impl(boost::mutex::scoped_lock& l, const int request_id, Args&&... args)
    {
        while (!m_sched.try_lock(request_id, std::forward<Args>(args)...))
            m_cond.wait(l);
        return true;
    }

    template <typename... Args>
    bool try_lock_impl(boost::mutex::scoped_lock&, const int request_id, Args&&... args)
    {
        return m_sched.try_lock(request_id, std::forward<Args>(args)...);
    }

    /// \todo Throw \c boost::thread_resource_error w/ some meaningful error condition
    template <typename... Args>
    bool lock_decorator(lock_func_t<Args...> lf, Args&&... args)
    {
        bool result = false;
        {
            boost::mutex::scoped_lock l(m_mut);             // may throw boost::thread_resource_error
            /// - Ask for unique request ID from the underlaid schduler
            int request_id;
            try
            {
                request_id = m_sched.assign_request_id(std::forward<Args>(args)...);
            }
            catch (const boost::thread_interrupted&)
            {
                throw;
            }
            catch (...)
            {
                throw boost::thread_resource_error();
            }
            /// - Call decorated member-function to acquire a lock, passing
            ///   all given params, including obtained request ID (as the first one)
            try
            {
                result = (this->*lf)(l, request_id, std::forward<Args>(args)...);
            }
            catch (const boost::thread_interrupted& e)
            {
                try
                {
                    m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
                }
                catch (...)
                {
                    // ignore possible errors! original exception is more important!
                }
                throw e;                                    // rethrow original thread_interrupted anyway
            }
            /// - Unassign request ID
            try
            {
                m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
            }
            catch (const boost::thread_interrupted&)
            {
                throw;
            }
            catch (...)
            {
                throw boost::thread_resource_error();
            }
        }
        /// - Notify other waiters
        m_cond.notify_all();
        return result;
    }

    boost::mutex m_mut;
    boost::condition_variable m_cond;
    scheduler_type m_sched;
};

}}                                                          // namespace thread, zencxx
#endif                                                      // __ZENCXX__THREAD__UNILOCK_HH__
