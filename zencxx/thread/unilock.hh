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
# include <zencxx/details/export.hh>

// Standard includes
# include <boost/thread/condition_variable.hpp>
# include <boost/thread/mutex.hpp>
# include <utility>

namespace zencxx {
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
    using lock_func_t = bool (unilock::*)(int, boost::mutex::scoped_lock&, Args&&...);

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
    bool lock_impl(const int request_id, boost::mutex::scoped_lock& l, Args&&... args)
    {
        while (!m_sched.try_lock(request_id, std::forward<Args>(args)...))
            m_cond.wait(l);
        return true;
    }

    template <typename... Args>
    bool try_lock_impl(const int request_id, boost::mutex::scoped_lock&, Args&&... args)
    {
        return m_sched.try_lock(request_id, std::forward<Args>(args)...);
    }

    template <typename... Args>
    bool lock_decorator(lock_func_t<Args...> lf, Args&&... args)
    {
        bool result = false;
        {
            boost::mutex::scoped_lock l(m_mut);             // may throw boost::thread_resource_error
            int request_id;
            try
            {
                request_id = m_sched.assign_request_id(std::forward<Args>(args)...);
            }
            catch (...)
            {
                throw boost::thread_resource_error();
            }
            try
            {
                // call decorated member function
                result = (this->*lf)(request_id, l, std::forward<Args>(args)...);
            }
            catch (const boost::thread_interrupted& e)
            {
                try
                {
                    m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
                }
                catch (...)
                {
                    throw e;                                // rethrow thread_interrupted anyway
                }
            }
            try
            {
                m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
            }
            catch (...)
            {
                throw boost::thread_resource_error();
            }
        }
        m_cond.notify_all();
        return result;
    }

    boost::mutex m_mut;
    boost::condition_variable m_cond;
    scheduler_type m_sched;
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__THREAD__UNILOCK_HH__
