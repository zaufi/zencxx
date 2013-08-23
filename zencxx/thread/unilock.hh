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
# include <zencxx/thread/exception.hh>

// Standard includes
# include <boost/thread/condition_variable.hpp>
# include <boost/thread/mutex.hpp>
# include <utility>

namespace zencxx { inline namespace thread {
/**
 * \brief Unilock - a universal lock.
 *
 * Unilock is an attempt to create a modular mechanism for creating lock
 * managers with different characteristics. Unilock is constructed from three
 * independent parts: high level wrapper (the unilock class itself), scheduler
 * (which is passed as a template argument) and lock compatibility matrix
 * (passed as a template parameter to the scheduler).
 *
 * \todo It seems better to pass variadic parameters by \b value instead
 * of "perfect" forwarding.
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
        catch (const exception&)
        {
            throw;
        }
        catch (const boost::lock_error& e)
        {
            ZENCXX_THROW(unilock_exception::unilock_error(e.code()))
              << exception::reason(e.what());
        }
        m_cond.notify_all();
    }

private:
    template <typename... Args>
    bool lock_impl(boost::mutex::scoped_lock& l, const int request_id, Args&&... args)
    {
        while (!m_sched.try_lock(details::use_deadlock_check::yes, request_id, std::forward<Args>(args)...))
            m_cond.wait(l);
        return true;
    }

    template <typename... Args>
    bool try_lock_impl(boost::mutex::scoped_lock&, const int request_id, Args&&... args)
    {
        return m_sched.try_lock(details::use_deadlock_check::no, request_id, std::forward<Args>(args)...);
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
                ZENCXX_THROW(unilock_exception::unilock_error());
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
                ZENCXX_THROW(unilock_exception::unilock_error());
            }
        }
        /// - Notify other waiters
        try
        {
            m_cond.notify_all();
        }
        catch (...)
        {
            ZENCXX_THROW(unilock_exception::unilock_error());
        }
        return result;
    }

    boost::mutex m_mut;
    boost::condition_variable m_cond;
    scheduler_type m_sched;
};

}}                                                          // namespace thread, zencxx
#endif                                                      // __ZENCXX__THREAD__UNILOCK_HH__
