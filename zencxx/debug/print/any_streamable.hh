/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::details::any_streamable (interface)
 *
 * \date Sun Jul 28 13:48:01 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__ANY_STREAMABLE_HH__
# define __ZENCXX__DEBUG__PRINT__ANY_STREAMABLE_HH__

// Project specific includes
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/print/any_wrapper.hh>
# include <zencxx/type_traits/has_left_shift.hh>

// Standard includes
# include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_streamable : public any_wrapper<T>
{
    static_assert(
        has_left_shift<std::ostream&, T>::value
      , "Type T must be streamable"
      );
    using any_wrapper<T>::any_wrapper;
};

/// Make a wrapper type streamable
template <typename T>
std::ostream& operator<<(std::ostream& os, const any_streamable<T>& aw)
{
    os << aw.data();
    // Show type info if needed
    details::show_type_info_impl<typename any_streamable<T>::wrapped_type>(os);
    return os;
}
}}}}                                                        // namespace details, print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__ANY_STREAMABLE_HH__
