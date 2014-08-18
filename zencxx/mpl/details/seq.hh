/**
 * \file
 *
 * \brief Class \c zencxx::mpl::seq (interface)
 *
 * \date Mon Jul  2 13:41:02 MSK 2012 -- Initial design
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
#include <zencxx/mpl/details/variadic_sequence_tag.hh>
#include <zencxx/mpl/seq_fwd.hh>

// Standard includes
#include <boost/mpl/long.hpp>

namespace zencxx { namespace mpl {

/**
 * \brief Compile-time sequence type
 *
 * This class is limitless version of \c boost::mpl::seq. It completely compatible
 * w/ boost MPL library containers, but based on true variadic templates.
 *
 */
template <typename... Types>
struct seq
{
  typedef seq type;
  typedef details::variadic_sequence_tag tag;
  typedef boost::mpl::long_<sizeof...(Types)> size;
};

}}                                                          // namespace mpl, zencxx
