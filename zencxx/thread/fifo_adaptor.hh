/**
 * \file
 *
 * \brief Class \c zencxx::thread::fifo_adaptor (interface)
 *
 * \date Thu Aug 22 15:18:45 MSK 2013 -- Initial design
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

#pragma once

// Project specific includes
#include <zencxx/thread/details/use_deadlock_check.hh>

// Standard includes
#include <algorithm>
#include <cassert>
#include <vector>

namespace zencxx { namespace thread {

/**
 * \brief Simple FIFO locking policy (adaptor)
 *
 * This adaptor organize lock acquire requests in order they appear in a FIFO queue.
 *
 */
template <typename Scheduler>
class fifo_adaptor
{
    typedef Scheduler scheduler_type;

public:
    /// This adaptor add a \c priority parameter to this member
    template <typename... Args>
    bool try_lock(
        const details::use_deadlock_check check
      , const int request_id
      , Args&&... args
      )
    {
        // Lock (try to) if top request ID equal to a given one
        // and underlaid scheduler can lock
        return (*begin(m_queue) == request_id)
          && m_sched.try_lock(check, request_id, std::forward<Args>(args)...);
    }

    /// This adaptor add no parameters to this method
    template <typename... Args>
    void unlock(Args&&... args)
    {
        m_sched.unlock(std::forward<Args>(args)...);
    }

    template <typename... Args>
    int assign_request_id(Args&&... args)
    {
        const auto rid = m_sched.assign_request_id(std::forward<Args>(args)...);
        m_queue.emplace_back(rid);                          // Remember for future references
        return rid;
    }

    template <typename... Args>
    void unassign_request_id(const int request_id, Args&&... args)
    {
        // Find a given request ID in a queue
        auto it = std::find(begin(m_queue), end(m_queue), request_id);
        assert("Request expected to be in the queue" && it != end(m_queue));
        m_queue.erase(it);
        // Call underlaid scheduler
        m_sched.unassign_request_id(request_id, std::forward<Args>(args)...);
    }

    /// \c lock() and \c unlock() parameters depend on underlaid scheduler
    static constexpr bool symmetric_lock_unlock = scheduler_type::symmetric_lock_unlock;

private:
    scheduler_type m_sched;                                 ///< Underlaid scheduler instance
    std::vector<int> m_queue;
};

}}                                                          // namespace thread, zencxx
