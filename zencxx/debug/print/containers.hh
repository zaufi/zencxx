/**
 * \file
 *
 * \brief Debug printer for container types
 *
 * \date Fri Jul 26 00:40:00 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__CONTAINERS_HH__
# define __ZENCXX__DEBUG__PRINT__CONTAINERS_HH__

// Project specific includes
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/print/any_fwd.hh>
# include <zencxx/debug/print/any_wrapper.hh>
# include <zencxx/details/export.hh>
# include <zencxx/type_traits/is_range_iterable.hh>
# include <zencxx/type_traits/is_std_basic_string.hh>

// Standard includes
# include <ostream>
# include <string>
# include <type_traits>
# include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_container : public any_wrapper<T>
{
    static_assert(
        is_range_iterable<T>::value
      , "Type T must be iterable"
      );
    using any_wrapper<T>::any_wrapper;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const any_container<T>& wc)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << '{';
        bool first_iteration = true;
        for (const auto& item : wc.data())
        {
            if (!first_iteration)
                os << ", ";
            os << any(item);
            first_iteration = false;
        }
        os << '}';
    }
    // Show type info if needed
    details::show_type_info_impl<T>(os);
    return os;
}

template <typename T>
struct any_string : public any_wrapper<T>
{
    static_assert(
        is_std_basic_string<typename std::decay<T>::type>::value
      , "Type T must be std::basic_string instance"
      );
    using any_wrapper<T>::any_wrapper;
};

ZENCXX_EXPORT std::ostream& operator<<(std::ostream& os, const any_string<const std::string&>&);

}}}}                                                        // namespace details, print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__CONTAINERS_HH__
