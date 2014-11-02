/**
 * \file
 *
 * \brief Class \c zencxx::thread::details::scoped_lock_impl (interface)
 *
 * \date Wed Feb 26 12:50:52 MSK 2014 -- Initial design
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

// Standard includes
#include <cassert>
#include <functional>
#include <tuple>

namespace zencxx { inline namespace thread { namespace details {
/// \internal Class to hold a bunch of non-type (\c std::size_t) template params
/// used as indices to "expand" a \c tuple elements
template <std::size_t... Indices>
struct indices_seq
{
    typedef indices_seq type;
};

/// \internal Generic template to generate a sequence of indices
template <std::size_t Idx, std::size_t... Indices>
struct generate_indices_seq
  : public generate_indices_seq<Idx - 1, Idx - 1, Indices...>
{
};

/// \internal Terminal specialization of \c generate_indices_seq
template <std::size_t... Indices>
struct generate_indices_seq<0, Indices...>
{
    typedef indices_seq<Indices...> type;
};

/**
 * \brief Class \c unlock_invoker (generic version)
 *
 * Used to bind a parameters to be used later to call
 * \c unilock::unlock() function.
 *
 * \note The reason to use this class instead of \c std::bind
 * is that latter has a copy of passed parameters and at the
 * call time do not use perfect forwarding making it impossible
 * to bind a functions/methods w/ a ref-ref arguments.
 */
template <typename Unilock, typename... Args>
class unlock_invoker
{
public:
    unlock_invoker(Unilock& lockable, Args&&... args)
      : m_lockable{&lockable}
      , m_args{std::forward<Args>(args)...}
    {
    }
    void operator()()
    {
        invoke(typename generate_indices_seq<sizeof...(Args)>::type{});
    }

private:
    template <std::size_t... Indices>
    void invoke(indices_seq<Indices...> /* dummy */)
    {
        m_lockable->unlock(
            std::forward<
                typename std::tuple_element<Indices, decltype(m_args)>::type
              >(std::get<Indices>(m_args))...
          );
    }

    Unilock* m_lockable;
    std::tuple<Args...> m_args;
};

/**
 * \brief Parameterless specialization of \c unlock_invoker
 */
template <typename Unilock>
class unlock_invoker<Unilock>
{
public:
    unlock_invoker(Unilock& lockable)
      : m_lockable{&lockable}
    {
    }
    void operator()()
    {
        m_lockable->unlock();
    }

private:
    Unilock* m_lockable;
};

/**
 * \brief Implementation of \c unilock::scoped_lock with asymmetric schedulers
 *
 * \em Asymmetric means that type and/or count parameters to \c lock()
 * and \c unlock() are different.
 *
 * \internal Used by \c zencxx::thread::unilock to declare a nested \c scoped_lock type
 *
 * \todo TBD
 *
 */
template <typename Unilock, bool IsSymmetricLockAndUnlockParams>
class scoped_lock_impl
{
    /// \todo Good implementation required
};

/**
 * \brief Implementation of \c unilock::scoped_lock with symmetric schedulers
 *
 * \em Symmetric means that type and/or count parameters to \c lock()
 * and \c unlock() are the same
 *
 * \internal Used by \c zencxx::thread::unilock to declare a nested \c scoped_lock type
 */
template <typename Unilock>
class scoped_lock_impl<Unilock, true>
{
public:
    /// Lock a given \c Unilock instance passing given parameters
    template <typename... Args>
    scoped_lock_impl(Unilock& lockable, Args&&... args)
      : m_unlocker(unlock_invoker<Unilock, Args...>(lockable, std::forward<Args>(args)...))
    {
        lockable.lock(std::forward<Args>(args)...);
    }
    /// Unlock when this instance goes out of scope
    ~scoped_lock_impl()
    {
        assert("Sanity check" && m_unlocker);
        m_unlocker();
    }

private:
    /// Functor w/ bound parameters to be executed on destroy
    std::function<void()> m_unlocker;
};

}}}                                                         // namespace details, thread, zencxx
