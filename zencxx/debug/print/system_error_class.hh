/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::details::system_error_class (interface)
 *
 * \date Tue Oct 15 04:00:30 MSK 2013 -- Initial design
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
#include <zencxx/type_traits/is_system_error_class.hh>

// Standard includes
#include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_error_code_class : public any_wrapper<T>
{
    static_assert(
        is_system_error_class<typename std::decay<T>::type>::value
      , "Type T is not an error_code class"
      );
    using any_wrapper<T>::any_wrapper;
};

/// Make \c any streamable
template <typename T>
std::ostream& operator<<(std::ostream& os, const any_error_code_class<T>& error)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << error.data().message()
           << " (" << error.data().category().name() << ':' << error.data().value() << ')';
    }
    // Show type info if needed
    details::show_type_info_impl<typename any_error_code_class<T>::wrapped_type>(os);
    return os;
}

}}}}                                                        // namespace details, print, debug, zencxx
