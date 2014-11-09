/**
 * \file
 *
 * \brief Class \c zencxx::thread::priority_queue_adaptor
 *
 * \date Wed Aug 21 03:03:02 MSK 2013 -- Initial design
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
#include <algorithm>
#include <cassert>
#include <map>

namespace zencxx { namespace thread {
/**
 * \brief Simple prioritized FIFO locking adaptor.
 *
 * In this scheduler (adaptor actually) request are sorted by their priority.
 * The bigger priority is the more chances the request gets.
 * Requests with equal priority are served in FIFO order.
 * \e Adaptor means that this class instantiated w/ a \e real scheduler or
 * another adaptor.
 *
 * \tparam Scheduler a wrapped (adopted) scheduler class
 *
 * \todo Use \c boost::multiindex_container instead of \c std::multimap?
 */
template <typename Scheduler>
class priority_queue_adaptor
{
    typedef Scheduler scheduler_type;

public:
    /// This adaptor add a \c priority parameter to this member
    template <typename... Args>
    bool try_lock(
        const details::use_deadlock_check check
      , const int request_id
      , const int priority
      , Args&&... args
      )
    {
        // Lock (try to) if top request ID equal to a given one
        if (begin(m_queue)->second == request_id)
        {
            assert("Verify priority" && priority == begin(m_queue)->first);
            // Delegate call to underlaid scheduler
            return m_sched.try_lock(check, request_id, std::forward<Args>(args)...);
        }
        // Can't handle this request now, more prioritized requests are pending...
        return false;
    }

    /// This adaptor add no parameters to this method
    template <typename... Args>
    void unlock(Args&&... args)
    {
        m_sched.unlock(std::forward<Args>(args)...);
    }

    template <typename... Args>
    int assign_request_id(const int priority, Args&&... args)
    {
        const auto rid = m_sched.assign_request_id(std::forward<Args>(args)...);
        m_queue.emplace(priority, rid);                     // Remember for future references
        return rid;
    }

    /// \todo Need benchmarks of this method to decide is \c boost::multiindex_container
    /// can beat this straightforward (naive) approach...
    template <typename... Args>
    void unassign_request_id(const int request_id, const int priority, Args&&... args)
    {
        // Find a range of requests w/ same priority
        auto range = m_queue.equal_range(priority);
        assert("Request expected to be in the queue" && range.first != end(m_queue));
        auto it = std::find_if(
            range.first
          , range.second
          , [request_id](const std::pair<const int, int>& p)
            {
                return request_id == p.second;
            }
          );
        assert("Request expected to be in the queue" && it != end(m_queue));
        m_queue.erase(it);
        // Call underlaid scheduler
        m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
    }

    /// \c lock() and \c unlock() parameters are not the same for this schduler
    static constexpr bool symmetric_lock_unlock = false;

private:

    /// Lock/unlock requests queue: map priority to request ID
    std::multimap<int, int, std::greater<int>> m_queue;
    scheduler_type m_sched;                                 ///< Underlaid scheduler instance
};

}}                                                          // namespace thread, zencxx
