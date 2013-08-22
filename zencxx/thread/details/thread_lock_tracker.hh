/**
 * \file
 *
 * \brief Class \c zencxx::thread::details::thread_lock_tracker (interface)
 *
 * \date Wed Aug 21 15:46:09 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__THREAD__DETAILS__THREAD_LOCK_TRACKER_HH__
# define __ZENCXX__THREAD__DETAILS__THREAD_LOCK_TRACKER_HH__

// Project specific includes
# include <zencxx/details/export.hh>
# include <zencxx/thread/details/use_deadlock_check.hh>
# include <zencxx/thread/exception.hh>
# include <zencxx/thread/predefined_lock_types.hh>

// Standard includes
# include <boost/thread/thread.hpp>
# include <array>
# include <vector>

namespace zencxx { inline namespace thread { namespace details {

/**
 * \brief Class to track thread IDs and corresponding lock state
 *
 * [More detailed description here]
 *
 */
template <
    typename MatrixSpec
  , std::size_t StateSize = std::extent<decltype(MatrixSpec::matrix)>::value
  >
class thread_lock_tracker
{
    typedef MatrixSpec matrix_type;
    typedef typename matrix_type::type lock_type;

public:
    thread_lock_tracker()
      : m_lock_holders{{}}
    {}
    ~thread_lock_tracker() {}

    /// Delete copy ctor
    thread_lock_tracker(const thread_lock_tracker&) = delete;
    /// Delete copy-assign operator
    thread_lock_tracker& operator=(const thread_lock_tracker&) = delete;
    /// Delete move ctor
    thread_lock_tracker(thread_lock_tracker&&) = delete;
    /// Delete move-assign operator
    thread_lock_tracker& operator=(thread_lock_tracker&&) = delete;

    /// Check if given lock type already acquired by this thread
    bool is_locked_by_this_thread(const lock_type t) const
    {
        const auto& holders = m_lock_holders[static_cast<std::size_t>(t)];
        return std::find(begin(holders), end(holders), boost::this_thread::get_id()) != end(holders);
    }
    /// Set current thread ID as a holder of a lock of given type
    void remember_lock_holder(use_deadlock_check check, const lock_type t)
    {
        if (check == use_deadlock_check::yes && is_locked_by_this_thread(t))
            ZENCXX_THROW(unilock_exception::deadlock());
        m_lock_holders[static_cast<std::size_t>(t)].emplace_back(boost::this_thread::get_id());
    }
    /// Unset current thread ID as a holder of a lock of given type
    void forget_lock_holder(const lock_type t)
    {
        auto& holders = m_lock_holders[static_cast<std::size_t>(t)];
        auto it = std::find(begin(holders), end(holders), boost::this_thread::get_id());
        if (it != end(holders))
            holders.erase(it);
        else
            ZENCXX_THROW(unilock_exception::not_owner_of_lock());
    }

private:
    std::array<std::vector<boost::thread::id>, StateSize> m_lock_holders;
};

template <typename MatrixSpec>
class thread_lock_tracker<MatrixSpec, 1ul>
{
    typedef MatrixSpec matrix_type;
    typedef typename matrix_type::type lock_type;

public:
    thread_lock_tracker()
      : m_lock_holder{}
    {}
    ~thread_lock_tracker() {}

    /// Delete copy ctor
    thread_lock_tracker(const thread_lock_tracker&) = delete;
    /// Delete copy-assign operator
    thread_lock_tracker& operator=(const thread_lock_tracker&) = delete;
    /// Delete move ctor
    thread_lock_tracker(thread_lock_tracker&&) = delete;
    /// Delete move-assign operator
    thread_lock_tracker& operator=(thread_lock_tracker&&) = delete;

    /// Check if given (and the only) lock type already acquired by this thread
    bool is_locked_by_this_thread(const lock_type) const
    {
        return m_lock_holder == boost::this_thread::get_id();
    }
    /// Set current thread ID as a holder of a lock of given type
    void remember_lock_holder(use_deadlock_check check, const lock_type t)
    {
        if (check == use_deadlock_check::yes && is_locked_by_this_thread(t))
            ZENCXX_THROW(unilock_exception::deadlock());
        m_lock_holder = boost::this_thread::get_id();
    }
    /// Unset current thread ID as a holder of a lock of given type
    void forget_lock_holder(const lock_type t)
    {
        if (is_locked_by_this_thread(t))
            m_lock_holder = {};
        else
            ZENCXX_THROW(unilock_exception::not_owner_of_lock());
    }

private:
    boost::thread::id m_lock_holder = {};
};

extern ZENCXX_EXPORT template class thread_lock_tracker<exclusive_lock, 1ul>;
extern ZENCXX_EXPORT template class thread_lock_tracker<rw_lock, 2ul>;

}}}                                                         // namespace details, thread, zencxx

extern ZENCXX_EXPORT template class std::vector<boost::thread::id>;
extern ZENCXX_EXPORT template class std::array<std::vector<boost::thread::id>, 2ul>;

#endif                                                      // __ZENCXX__THREAD__DETAILS__THREAD_LOCK_TRACKER_HH__
