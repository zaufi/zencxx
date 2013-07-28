/**
 * \file
 *
 * \brief Metafunctions to check for \c to_string() member or free functions availability
 *
 * \date Thu Jul 25 09:31:27 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2013 Alex Turbov and contributors, all rights reserved.
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

#ifndef __ZENCXX__TYPE_TRAITS__TO_STRING_HH__
# define __ZENCXX__TYPE_TRAITS__TO_STRING_HH__

// Project specific includes
# include <zencxx/type_traits/details/expression_validity_checker.hh>

// Standard includes
# include <string>

namespace zencxx {

/// Generate \c has_to_string_member metafunction
ZENCXX_TT_EXPR_CHECKER_EX(
    has_to_string_member
  , (typename T)
  , (std::string& out_str)
  , (T)
  , (std::declval<std::string&>())
  , out_str = std::declval<const T&>().to_string()
  );

/// Generate \c has_to_string_adl metafunction
ZENCXX_TT_EXPR_CHECKER_EX(
    has_to_string_adl
  , (typename T)
  , (std::string& out_str)
  , (T)
  , (std::declval<std::string&>())
  , out_str = to_string(std::declval<const T&>())
  );

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__TYPE_TRAITS__TO_STRING_HH__
