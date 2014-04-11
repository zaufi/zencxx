/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::details::any_convertible_to_string (interface)
 *
 * \date Sun Jul 28 14:09:35 MSK 2013 -- Initial design
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
#include <zencxx/debug/print/any_manip.hh>
#include <zencxx/debug/print/any_wrapper.hh>
#include <zencxx/type_traits/to_string.hh>

// Standard includes
#include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {

template <
    typename T
  , bool ConvertibleViaMember = has_to_string_member<T>::value
  , bool ConvertibleViaADL = has_to_string_adl<T>::value
  >
struct converter;

template <typename T, bool AnyValueIsOk>
struct converter<T, true, AnyValueIsOk>
{
    static std::string do_conversion(const T& t)
    {
        return t.to_string();
    }
};

template <typename T>
struct converter<T, false, true>
{
    static std::string do_conversion(const T& t)
    {
        return to_string(t);
    }
};

template <typename T>
struct any_convertible_to_string : public any_wrapper<T>
{
    static_assert(
        has_to_string_adl<T>::value || has_to_string_member<T>::value
      , "Type T must be convertible to string with to_string() member or free function"
      );
    using any_wrapper<T>::any_wrapper;
};

/// Make a wrapper type streamable
template <typename T>
std::ostream& operator<<(std::ostream& os, const any_convertible_to_string<T>& aw)
{
    os << converter<T>::do_conversion(aw.data());
    // Show type info if needed
    details::show_type_info_impl<typename any_convertible_to_string<T>::wrapped_type>(os);
    return os;
}

}}}}                                                        // namespace details, print, debug, zencxx
