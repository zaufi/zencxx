/**
 * \file
 *
 * \brief \c push_back, \c pop_back, \c push_front and \c pop_front implementations for variadic \c seq
 *
 * \date Mon Jul  2 15:43:16 MSK 2012 -- Initial design
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
#include <zencxx/mpl/details/seq.hh>
#include <zencxx/mpl/copy_range.hh>

// Standard includes
#include <boost/mpl/pop_back_fwd.hpp>
#include <boost/mpl/push_back_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>
#include <boost/mpl/pop_front_fwd.hpp>

namespace boost { namespace mpl {

/**
 * \brief \c boost::mpl::pop_back specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct pop_back_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::pop_back specialization
 *
 * ... just copy everything from \c begin to \c size-1 to a new container.
 *
 * \note Because better specialization doesn't works (dunno why), lets do a copy...
 */
template <>
template <typename... Types>
struct pop_back_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types...>>
{
    typedef typename zencxx::mpl::copy_range_c<
        zencxx::mpl::seq<Types...>
      , 0
      , sizeof...(Types) - 1
      >::type type;
};

/**
 * Specialization for last item in a sequence... just to avoid more template instantiations.
 */
template <>
template <typename T>
struct pop_back_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<T>>
{
    typedef typename zencxx::mpl::seq<> type;
};

/**
 * \brief \c boost::mpl::push_back specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct push_back_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq, typename T>
    struct apply;
};

/**
 * \brief \c boost::mpl::push_back specialization for \c zencxx::mpl::seq
 */
template <>
template <typename... Types, typename T>
struct push_back_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types...>, T>
{
    typedef zencxx::mpl::seq<Types..., T> type;
};

/**
 * \brief \c boost::mpl::pop_front specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct pop_front_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::push_front specialization for \c zencxx::mpl::seq
 */
template <>
template <typename T, typename... Types>
struct pop_front_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<T, Types...>>
{
    typedef zencxx::mpl::seq<Types...> type;
};

/**
 * \brief \c boost::mpl::push_front specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct push_front_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq, typename T>
    struct apply;
};

/**
 * \brief \c boost::mpl::push_front specialization for \c zencxx::mpl::seq
 */
template <>
template <typename... Types, typename T>
struct push_front_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types...>, T>
{
    typedef zencxx::mpl::seq<Types..., T> type;
};

}}                                                          // namespace mpl, boost
