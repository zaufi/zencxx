/**
 * \file
 *
 * \brief \c zencxx::mpl::has_type metafunction
 *
 * \date Wed Jul  4 05:07:08 MSK 2012 -- Initial design
 * \date Sun Nov  9 08:18:01 MSK 2014 -- Migrate to improved expression checker
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
#include <zencxx/type_traits/details/is_valid.hh>

// Standard includes
#include <utility>                                          // for std::declval

namespace zencxx { namespace mpl { namespace details {

template <typename T>
using has_type_t = decltype(std::declval<typename T::type>());

}                                                           // namespace details

/**
 * \attention Nowadays (boost <= 1.50) \c has_type metafunction placed in the
 * \c boost::mpl::aux, so we can't use it here (yep, it's undocumented
 * implementation details), so we have to define our own... and better one
 * because of new features of C++11 :))
 */
template <typename T>
struct has_type : ::zencxx::details::is_valid<details::has_type_t, T>
{};

}}                                                          // namespace mpl, zencxx
