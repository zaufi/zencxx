/**
 * \file
 *
 * \brief Debug printing for \c std::pair
 *
 * \date Tue Jul 23 02:00:06 MSK 2013 -- Initial design
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
#include <zencxx/debug/print/any_fwd.hh>
#include <zencxx/debug/print/any_wrapper.hh>
#include <zencxx/type_traits/is_std_pair.hh>

// Standard includes
#include <ostream>
#include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_pair : public any_wrapper<T>
{
    static_assert(
        is_std_pair<typename std::decay<T>::type>::value
      , "Type T must be an instance of std::pair"
      );
    using any_wrapper<T>::any_wrapper;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const any_pair<T>& pw)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << '(' << any(pw.data().first) << ',' << ' ' << any(pw.data().second) << ')';
    }
    // Show type info if needed
    details::show_type_info_impl<typename any_pair<T>::wrapped_type>(os);
    return os;
}

}}}}                                                        // namespace details, print, debug, zencxx
