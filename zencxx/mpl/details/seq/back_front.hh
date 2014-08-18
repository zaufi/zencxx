/**
 * \file
 *
 * \brief \c back and \c front implementation for variadic \c seq
 *
 * \date Mon Jul  2 15:43:06 MSK 2012 -- Initial design
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
#include <zencxx/mpl/seq_fwd.hh>
#include <zencxx/mpl/details/variadic_sequence_tag.hh>

// Standard includes
#include <boost/mpl/back_fwd.hpp>
#include <boost/mpl/front_fwd.hpp>

namespace boost { namespace mpl {
/**
 * \brief \c boost::mpl::back specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct back_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::back specialization for \c zencxx::mpl::seq
 */
template <>
template <typename... Types, typename T>
struct back_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<Types..., T>>
{
    typedef T type;
};

/**
 * \brief \c boost::mpl::front specialization
 *
 * \note Generic metaclass has no implementation
 */
template <>
struct front_impl<zencxx::mpl::details::variadic_sequence_tag>
{
    template <typename Seq>
    struct apply;
};

/**
 * \brief \c boost::mpl::front specialization for \c zencxx::mpl::seq
 */
template <>
template <typename T, typename... Types>
struct front_impl<zencxx::mpl::details::variadic_sequence_tag>::apply<zencxx::mpl::seq<T, Types...>>
{
    typedef T type;
};

}}                                                          // namespace mpl, boost
