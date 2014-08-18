/**
 * \file
 *
 * \brief Metafunction \c zencxx::mpl::v_at
 *
 * \date Sat Jun 30 22:23:42 MSK 2012 -- Initial design
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
#include <zencxx/mpl/details/v_at_impl.hh>

// Standard includes
#include <boost/mpl/long.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Variadic version of \c at
 *
 * Metafunction to get item at given index \c N from variadic sequence.
 *
 * \tparam N item index to get
 * \tparam Seq variadic sequence of elements
 *
 */
template <typename N, typename... Seq>
struct v_at : details::v_at_impl<N, boost::mpl::long_<0>, Seq...>
{};

/**
 * \brief Variadic version of \c at_c
 *
 * Metafunction to get item at given index \c N from variadic sequence.
 *
 * \tparam N item index to get
 * \tparam Seq variadic sequence of elements
 *
 */
template <long N, typename... Seq>
struct v_at_c : details::v_at_impl<boost::mpl::long_<N>, boost::mpl::long_<0>, Seq...>
{};

}}                                                          // namespace mpl, zencxx
