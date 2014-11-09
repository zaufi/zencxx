/**
 * \file
 *
 * \brief Class \c zencxx::thread::details::lock_matrix (interface)
 *
 * \date Wed Jul 17 11:33:06 MSK 2013 -- Initial design
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
#include <zencxx/thread/predefined_lock_types.hh>
#include <zencxx/thread/details/export.hh>

// Standard includes
#include <cassert>
#include <type_traits>

namespace zencxx { namespace thread { namespace details {

/**
 * \brief Lock compatibility matrix
 *
 * Generic template.
 *
 */
template <typename MatrixSpec>
class lock_matrix
{
    typedef MatrixSpec matrix_type;

    static constexpr std::size_t STATE_SIZE = std::extent<decltype(matrix_type::matrix)>::value;
    static constexpr std::size_t STATE_SIZE_1__ = std::extent<decltype(matrix_type::matrix), 1>::value;

    /// \todo Assert that matrix has \c bool values
    static_assert(
        STATE_SIZE == STATE_SIZE_1__
      , "`matrix' member of a MatrixSpec must be a NxN array of booleans"
      );
    static_assert(
        std::is_enum<typename matrix_type::type>::value
      , "MatrixSpec must have a nested enum `type'"
      );

    typedef typename matrix_type::type lock_type;

public:
    void lock(const lock_type t)
    {
        ++m_state[t];
    }
    void unlock(const lock_type t)
    {
        --m_state[t];
        /// \todo Need to throw smth here! (resourse/lock error?)
        assert("More unlock()s than lock()s??" && 0 <= m_state[t]);
    }
    constexpr bool can_lock(const lock_type t, const std::size_t n = STATE_SIZE - 1) const
    {
        return (!(matrix_type::matrix[t][n] || bool(m_state[n])) || matrix_type::matrix[t][n])
          && (n ? can_lock(t, n - 1) : true)
          ;
    }
    constexpr bool is_locked(const std::size_t n = STATE_SIZE - 1) const
    {
        return (0 < m_state[n]) || (n ? is_locked(n - 1) : false);
    }
    constexpr bool is_locked(const lock_type t) const
    {
        return 0 < m_state[t];
    }

private:
    int m_state[STATE_SIZE] = {};
};

template <>
class ZENCXXTHREAD_EXPORT lock_matrix<exclusive_lock>
{
    typedef exclusive_lock matrix_type;
    typedef typename matrix_type::type lock_type;

public:
    void lock(const lock_type)
    {
        ++m_state;
    }
    void unlock(const lock_type)
    {
        --m_state;
        /// \todo Need to throw smth here! (resourse/lock error?)
        assert("More unlock()s than lock()s??" && 0 <= m_state);
    }
    constexpr bool can_lock(const lock_type) const
    {
        return !is_locked();
    }
    constexpr bool is_locked() const
    {
        return (0 < m_state);
    }
    constexpr bool is_locked(const lock_type) const
    {
        return 0 < m_state;
    }

private:
    int m_state = {};
};

template <>
class ZENCXXTHREAD_EXPORT lock_matrix<rw_lock>
{
    typedef rw_lock matrix_type;
    typedef typename matrix_type::type lock_type;

    static constexpr std::size_t READ = 0;
    static constexpr std::size_t WRITE = 1;

public:
    void lock(const lock_type t)
    {
        ++m_state[t];
    }
    void unlock(const lock_type t)
    {
        --m_state[t];
        /// \todo Need to throw smth here! (resourse/lock error?)
        assert("More unlock()s than lock()s??" && 0 <= m_state[t]);
    }
    constexpr bool can_lock(const lock_type t) const
    {
        return ((t == matrix_type::read) && !bool(m_state[WRITE]))
            || ((t != matrix_type::read) && !bool(m_state[WRITE]) && !bool(m_state[READ]));
    }
    constexpr bool is_locked() const
    {
        return (0 < m_state[READ]) || (0 < m_state[WRITE]);
    }
    constexpr bool is_locked(const lock_type t) const
    {
        return 0 < m_state[t];
    }

private:
    int m_state[2] = {};
};

}}}                                                         // namespace details, thread, zencxx
