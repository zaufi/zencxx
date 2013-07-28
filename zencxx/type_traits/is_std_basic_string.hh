/**
 * \file
 *
 * \brief Class \c zencxx::is_std_basic_string (interface)
 *
 * \date Sun Jul 28 19:52:54 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__TYPE_TRAITS__IS_STD_BASIC_STRING_HH__
# define __ZENCXX__TYPE_TRAITS__IS_STD_BASIC_STRING_HH__

// Project specific includes

// Standard includes
# include <string>
# include <type_traits>

namespace zencxx {

/// Metafunction to check is given type is a \c std::basic_string (generic)
template <typename T>
struct is_std_basic_string : public std::false_type
{};

/// Metafunction to check is given type is a \c std::basic_string (specialized)
template <typename CharType, typename CharTraits, typename Alloc>
struct is_std_basic_string<
    std::basic_string<CharType, CharTraits, Alloc>
  > : public std::true_type
{};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__IS_STD_BASIC_STRING_HH__