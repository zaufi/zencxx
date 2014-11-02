/**
 * \file
 *
 * \brief Class \c zencxx::thread::parameterized_adaptor (interface)
 *
 * \date Fri Aug 23 06:42:31 MSK 2013 -- Initial design
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
#include <zencxx/thread/details/use_deadlock_check.hh>

// Standard includes
#include <boost/call_traits.hpp>
#include <cassert>
#include <map>

namespace zencxx { inline namespace thread {

/**
 * \brief Parameterized scheduler adds additional parameter to lock and unlock.
 *
 * A separate scheduler is created for every parameter passed to \c lock() as
 * an argument. After \c unlock(), if scheduler is not being used it is destroyed.
 *
 * \tparam Scheduler a wrapped (adopted) scheduler class to dispatch lock/unlock requests to
 * \tparam Key type of key used to dispatch requests to particular scheduler
 * \tparam KeyParam how to pass a \c Key values to a lock/unlock functions
 *
 * \todo Add a way to change an internal container type.
 * \todo Possible (likely) it is not a good idea to erase scheduler immediately
 * after \c unlock(). Need benchmarks anyway...
 */
template <
    typename Scheduler
  , typename Key
  , typename KeyParam = typename boost::call_traits<Key>::param_type
  >
class parameterized_adaptor
{
    typedef Scheduler scheduler_type;
    typedef Key key_type;
    typedef KeyParam key_param_type;

public:
    /// This adaptor add a \c Key parameter to this member
    template <typename... Args>
    bool try_lock(
        const details::use_deadlock_check check
      , const int request_id
      , const key_param_type key
      , Args&&... args
      )
    {
        // Try to find existed scheduler instance
        auto it = m_locks.find(key);
        assert(
            "Scheduler expected to be alive and valid!"
          && it != end(m_locks)
          && it->second.first
          );
        return it->second.second.try_lock(check, request_id, std::forward<Args>(args)...);
    }

    /// This adaptor add a \c Key parameter to this member
    template <typename... Args>
    void unlock(const key_param_type key, Args&&... args)
    {
        // Try to find existed scheduler instance
        auto it = m_locks.find(key);
        assert(
            "Scheduler expected to be alive and valid!"
          && it != end(m_locks)
          && it->second.first
          );
        it->second.second.unlock(std::forward<Args>(args)...);
        // Decrement use count and delete the scheduler if not used
        if (--it->second.first == 0) m_locks.erase(it);
    }

    template <typename... Args>
    int assign_request_id(const key_param_type key, Args&&... args)
    {
        // Try to find (and reuse) existed scheduler instance
        auto it = m_locks.find(key);
        if (it == end(m_locks))
            // Not found! Add it w/ use count == 1...
            it = m_locks.emplace(key, std::make_pair(1, scheduler_type())).first;
        else
        {
            assert("Scheduler expected to be used!" && it->second.first);
            // ALready here: just increment use count...
            ++it->second.first;
        }
        // Ask underlaid scheduler to assign a new request ID
        return it->second.second.assign_request_id(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void unassign_request_id(const int request_id, const key_param_type key, Args&&... args)
    {
        // Try to find existed scheduler instance
        auto it = m_locks.find(key);
        assert(
            "Scheduler expected to be alive and valid!"
          && it != end(m_locks)
          && it->second.first
          );
        // Delegate job to the underlaid scheduler
        it->second.second.unassign_request_id(request_id, std::forward<Args>(args)...);
    }

    /// \c lock() and \c unlock() parameters depend on underlaid scheduler
    static constexpr bool symmetric_lock_unlock = scheduler_type::symmetric_lock_unlock;

private:
    /// Lock/unlock requests map: key --> (use-count, scheduler)
    std::map<key_type, std::pair<int, scheduler_type>> m_locks;
};

}}                                                          // namespace thread, zencxx
