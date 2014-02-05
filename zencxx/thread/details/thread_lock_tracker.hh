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

#pragma once

// Project specific includes
#include <zencxx/thread/details/export.hh>
#include <zencxx/thread/details/use_deadlock_check.hh>
#include <zencxx/thread/exception.hh>
#include <zencxx/thread/predefined_lock_types.hh>

// Standard includes
#include <boost/thread/thread.hpp>
#include <array>
#include <vector>

namespace zencxx { inline namespace thread { namespace details {

/**
 * \brief Class to track thread IDs and corresponding lock state
 *
 * Generic template.
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
    std::array<std::vector<boost::thread::id>, StateSize> m_lock_holders = {{}};
};

/**
 * \brief Class to track thread IDs and corresponding lock state
 *
 * Specialzation for \c exclusive_lock
 *
 */
template <>
class ZENCXXTHREAD_EXPORT thread_lock_tracker<exclusive_lock, 1ul>
{
    typedef exclusive_lock matrix_type;
    typedef typename matrix_type::type lock_type;

public:
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

}}}                                                         // namespace details, thread, zencxx
