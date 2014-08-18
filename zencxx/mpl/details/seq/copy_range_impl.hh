/**
 * \file
 *
 * \brief Class \c zencxx::mpl::copy_range_impl (interface)
 *
 * \date Tue Jul  3 21:47:38 MSK 2012 -- Initial design
 */
/*
 * Copyright (C) 2012-2014 Alex Turbov and contributors, all rights reserved.
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
#include <zencxx/mpl/copy_range_fwd.hh>
#include <zencxx/mpl/details/variadic_sequence_tag.hh>

// Standard includes
#include <boost/mpl/apply.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/unpack_args.hpp>

namespace zencxx { namespace mpl { namespace details {

template <
    typename Current
  , typename Last
  , typename Inserter
  , typename... Types
  >
struct do_copy_range;

template <
    typename Current
  , typename Last
  , typename Inserter
  , typename T
  , typename... Types
  >
struct do_copy_range<Current, Last, Inserter, T, Types...>
{
    typedef typename boost::mpl::apply<
        boost::mpl::unpack_args<typename Inserter::operation>
      , mpl::seq<typename Inserter::state, T>
      >::type state_t;
    typedef typename do_copy_range<
        typename boost::mpl::next<Current>::type
      , Last
      , boost::mpl::inserter<state_t, typename Inserter::operation>
      , Types...
      >::type type;
};

template <
    typename Current
  , typename Inserter
  , typename T
  , typename... Types
  >
struct do_copy_range<Current, Current, Inserter, T, Types...>
{
    typedef typename Inserter::state type;
};

template <
    typename Current
  , typename Inserter
  >
struct do_copy_range<Current, Current, Inserter>
{
    typedef typename Inserter::state type;
};

template <
    typename Current
  , typename First
  , typename Last
  , typename Inserter
  , typename... Types
  >
struct skip_leading_items;

template <
    typename Current
  , typename First
  , typename Last
  , typename Inserter
  , typename T
  , typename... Types
  >
struct skip_leading_items<Current, First, Last, Inserter, T, Types...>
{
    typedef typename skip_leading_items<
        typename boost::mpl::next<Current>::type
      , First
      , Last
      , Inserter
      , Types...
      >::type type;
};

template <
    typename Current
  , typename Last
  , typename Inserter
  , typename T
  , typename... Types
  >
struct skip_leading_items<Current, Current, Last, Inserter, T, Types...>
{
    typedef typename do_copy_range<
        Current
      , Last
      , Inserter
      , T
      , Types...
      >::type type;
};

}                                                           // namespace details

/**
 * \brief \c copy_range implementation for variadic sequences
 *
 * \note Default metaclass do nothing
 */
template <>
struct copy_range_impl<details::variadic_sequence_tag>
{
    template <
        typename Seq
      , typename First
      , typename Last
      , typename Inserter
      >
    struct apply;
};

/**
 * \brief Specialization of \c copy_range_impl metaclass for variadic sequences
 */
template <>
template <
    typename... Types
  , typename First
  , typename Last
  , typename Inserter
  >
struct copy_range_impl<
    details::variadic_sequence_tag
  >::apply<
    seq<Types...>
  , First
  , Last
  , Inserter
  >
{
    static_assert(
        First::value < seq<Types...>::size::value
      , "index is out of range"
      );
    static_assert(
        Last::value <= seq<Types...>::size::value
      , "index is out of range"
      );

    typedef typename boost::mpl::eval_if<
        boost::mpl::less<boost::mpl::long_<0>, First>
      , details::skip_leading_items<
            boost::mpl::long_<0>
          , First
          , Last
          , Inserter
          , Types...
          >
      , details::do_copy_range<
            boost::mpl::long_<0>
          , Last
          , Inserter
          , Types...
          >
      >::type type;
};

/**
 * \brief Specialization of \c copy_range_impl metaclass for empty range
 *
 * ... just to reduce compile-time complexity and avoid unnecessary instantiations.
 */
template <>
template <
    typename... Types
  , typename First
  , typename Inserter
  >
struct copy_range_impl<
    details::variadic_sequence_tag
  >::apply<
    seq<Types...>
  , First
  , First
  , Inserter
  >
{
    typedef typename Inserter::state type;                  // Just return output sequence
};

}}                                                          // namespace mpl, zencxx
