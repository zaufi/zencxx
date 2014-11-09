/**
 * \file
 *
 * \brief Class \c zencxx::unilock (interface)
 *
 * \date Wed Jul 17 11:46:50 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
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

#pragma once

// Project specific includes
#include <zencxx/thread/exception.hh>
#include <zencxx/thread/details/scoped_lock_impl.hh>
#include <zencxx/thread/details/use_deadlock_check.hh>

// Standard includes
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <utility>
#if defined(__GNUC__)
# include <cxxabi.h>
#endif

namespace zencxx { namespace thread {
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
    using lock_func_t = bool (unilock::*)(boost::unique_lock<boost::mutex>&, int, Args&&...);

public:
    using scoped_lock = details::scoped_lock_impl<unilock, Scheduler::symmetric_lock_unlock>;

    /**
     * Lock the \c unilock instance passign given parameters to
     * the underlaid scheduler
     */
    template <typename... Args>
    void lock(Args&&... args)
    {
        lock_decorator(&unilock::lock_impl, std::forward<Args>(args)...);
    }

    /**
     * Try to lock the \c unilock instance passign given parameters to
     * the underlaid scheduler
     */
    template <typename... Args>
    bool try_lock(Args&&... args)
    {
        return lock_decorator(&unilock::try_lock_impl, std::forward<Args>(args)...);
    }

    /**
     * Unlock the \c unilock instance passign given parameters to
     * the underlaid scheduler.
     *
     * \todo Is thread interrupt handler needed? Really?
     */
    template <typename... Args>
    void unlock(Args&&... args)
    {
        try
        {
            // NOTE May throw boost::thread_resource_error
            auto l = boost::unique_lock<boost::mutex>{m_mut};
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
    bool lock_impl(boost::unique_lock<boost::mutex>& l, const int request_id, Args&&... args)
    {
        while (!m_sched.try_lock(details::use_deadlock_check::yes, request_id, std::forward<Args>(args)...))
            m_cond.wait(l);
        return true;
    }

    template <typename... Args>
    bool try_lock_impl(boost::unique_lock<boost::mutex>&, const int request_id, Args&&... args)
    {
        return m_sched.try_lock(details::use_deadlock_check::no, request_id, std::forward<Args>(args)...);
    }

    template <typename... Args>
    bool lock_decorator(lock_func_t<Args...> lf, Args&&... args)
    {
        auto result = false;
        try
        {
            // NOTE  may throw boost::thread_resource_error
            auto l = boost::unique_lock<boost::mutex>{m_mut};
            {
                /// - Ask for unique request ID from the underlaid schduler
                auto request_id = int{};
                try
                {
                    request_id = m_sched.assign_request_id(std::forward<Args>(args)...);
                }
                catch (...)
                {
                    translate_exception();
                }
                try
                {
                    /// - Call decorated member-function to acquire a lock, passing
                    ///   all given params, including obtained request ID (as the first one)
                    result = (this->*lf)(l, request_id, std::forward<Args>(args)...);
                }
                catch (...)
                {
                    try
                    {
                        // Emergency unassign request ID
                        m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
                    }
                    catch (...)
                    {
                        // Ignore possible errors
                    }
                    throw;                                  // Does it rethrows the outer exception?
                }
                try
                {
                    /// - Unassign request ID
                    m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
                }
                catch (...)
                {
                    translate_exception();
                }
            }
            /// - Notify other waiters
            m_cond.notify_all();                            // nothrow
        }
        catch (const unilock_exception&)
        {
            throw;
        }
        catch (...)
        {
            translate_exception();
        }
        return result;
    }

    [[noreturn]] static void translate_exception()
    {
#if defined(__GNUC__)
        assert(
            "No need to call this function w/o active exception"
          && 0 != abi::__cxa_current_exception_type()
          );
#endif
        try
        {
            throw;
        }
        catch (const boost::thread_interrupted&)
        {
            throw;
        }
        catch (...)
        {
            /// \todo Attach details
            ZENCXX_THROW(unilock_exception::unilock_error());
        }
        /// \todo Catch and translate \c boost::thread exceptions and get failure code
    }

    boost::mutex m_mut;
    boost::condition_variable m_cond;
    scheduler_type m_sched;
};

}}                                                          // namespace thread, zencxx
