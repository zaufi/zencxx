/**
 * \file
 *
 * \brief Class \c zencxx::thread::priority_scheduler
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
# include <zencxx/thread/details/has_default_lock_param.hh>
# include <zencxx/thread/details/lock_matrix.hh>

// Standard includes
# include <algorithm>
# include <cassert>
# include <map>

namespace zencxx { inline namespace thread { namespace details {
/**
 * \brief Simple prioritized FIFO locking scheduler.
 *
 * Generic implementation assume that \c MatrixSpec has more than one
 * lock type (i.e. \c lock_type can't be default parameter).
 *
 * \tparam MatrixSpec a lock matrix type
 *
 * \todo Use \c boost::multiindex_container instead of \c std::multimap?
 */
template <typename MatrixSpec, bool HasDefaultLockParam>
class priority_scheduler_impl
{
public:
    typedef MatrixSpec matrix_type;
    typedef typename matrix_type::type lock_type;

    bool try_lock(const use_deadlock_check, const int request_id, const int priority, const lock_type p)
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
    lock_matrix<matrix_type> m_matrix;
    int m_next_request_id = {};
};

/**
 * \brief Specialization of \c priority_scheduler_impl for
 * \c MatrixSpec types w/ default lock type specified.
 */
template <typename MatrixSpec>
class priority_scheduler_impl<MatrixSpec, true>
  : public priority_scheduler_impl<MatrixSpec, false>
{
    typedef priority_scheduler_impl<MatrixSpec, false> base_class;

public:
    typedef typename base_class::matrix_type matrix_type;

    // Bring inherited members into the scope
    using base_class::try_lock;
    using base_class::unlock;
    using base_class::is_used;
    using base_class::assign_request_id;
    using base_class::unassign_request_id;

    bool try_lock(const use_deadlock_check check, const int request_id, const int priority)
    {
        return this->try_lock(check, request_id, priority, matrix_type::default_lock);
    }
    void unlock()
    {
        this->unlock(matrix_type::default_lock);
    }
    int assign_request_id(const int priority)
    {
        return this->assign_request_id(priority, matrix_type::default_lock);
    }
    void unassign_request_id(const int request_id, const int priority)
    {
        this->unassign_request_id(request_id, priority, matrix_type::default_lock);
    }
};
}                                                           // namespace details

/**
 * \brief Simple prioritized FIFO locking scheduler.
 *
 * In this scheduler request are sorted by their priority.
 * The bigger priority is the more chances the request gets.
 * Requests with equal priority are served in FIFO order.
 *
 * \tparam MatrixSpec a lock matrix type
 */
template <typename MatrixSpec>
class priority_scheduler
  : public details::priority_scheduler_impl<
      MatrixSpec
    , details::has_default_lock_param<MatrixSpec>::value
    >
{};

}}                                                          // namespace thread, zencxx
#endif                                                      // __ZENCXX__THREAD__PRIORITY_SCHEDULER_HH__
