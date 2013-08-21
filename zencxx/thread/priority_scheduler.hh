/**
 * \file
 *
 * \brief Class \c zencxx::thread::priority_scheduler (interface)
 *
 * \date Wed Aug 21 03:03:02 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__THREAD__PRIORITY_SCHEDULER_HH__
# define __ZENCXX__THREAD__PRIORITY_SCHEDULER_HH__

// Project specific includes
# include <zencxx/thread/details/lock_matrix.hh>

// Standard includes
# include <algorithm>
# include <cassert>
# include <map>

namespace zencxx { inline namespace thread {

/**
 * \brief Simple prioritized FIFO locking scheduler.
 *
 * In this scheduler request are sorted by their priority.
 * The bigger priority is the more chances the request gets.
 * Requests with equal priority are served in FIFO order.
 *
 * \tparam MatrixSpec a lock matrix type
 *
 * \todo Use \c boost::multiindex_container instead of \c std::multimap?
 */
template <typename MatrixSpec>
class priority_scheduler
{
public:
    typedef MatrixSpec matrix_type;
    typedef typename matrix_type::type lock_type;

    bool try_lock(const int request_id, const int priority, const lock_type p)
    {
        // Lock if top request ID equal to a given one and a matrix tells that
        // lock is possible/compatible.
        const auto result = begin(m_queue)->second == request_id && m_matrix.can_lock(p);
        if (result)
        {
            assert("Verify priority" && priority == begin(m_queue)->first);
            m_matrix.lock(p);
        }
        return result;
    }
    void unlock(const lock_type p)
    {
        m_matrix.unlock(p);
    }
    //
    bool is_used() const
    {
        return !m_queue.empty() || m_matrix.is_locked();
    }
    int assign_request_id(const int priority, const lock_type)
    {
        int rid = m_next_request_id++;
        m_queue.emplace(priority, rid);
        return rid;
    }
    /// \todo Need benchmarks of this method to decide is \c boost::multiindex_container
    /// can beat this straightforward (naive) approach...
    void unassign_request_id(const int request_id, const int priority, const lock_type)
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
    }

private:
    /// Lock/unlock requests queue: map priority to request ID
    std::multimap<int, int, std::greater<int>> m_queue;
    details::lock_matrix<matrix_type> m_matrix;
    int m_next_request_id = {};
};

}}                                                          // namespace thread, zencxx
#endif                                                      // __ZENCXX__THREAD__PRIORITY_SCHEDULER_HH__
