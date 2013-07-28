/**
 * \file
 *
 * \brief Debug printing for \c std::pair
 *
 * \date Tue Jul 23 02:00:06 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__STD_PAIR_HH__
# define __ZENCXX__DEBUG__PRINT__STD_PAIR_HH__

// Project specific includes
# include <zencxx/io_manipulator_wrapper.hh>
# include <zencxx/debug/print/any_generic.hh>

// Standard includes
# include <ostream>
# include <utility>

namespace zencxx { namespace debug { namespace print { namespace details {
ZENCXX_MAKE_TEMPLATE_TEMPLATE_WRAPPER_CLASS(std_pair_wrapper, std::pair);

template <typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& os, std_pair_wrapper<T1, T2>&& pw)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        os << '(' << any(pw.ref().first) << ',' << ' ' << any(pw.ref().second) << ')';
    }
    // Show type info if needed
    details::show_type_info_impl<typename std_pair_wrapper<T1, T2>::wrapped_type>(os);
    return os;
}
}                                                           // namespace details

template <typename T1, typename T2>
inline details::std_pair_wrapper<T1, T2> any(const std::pair<T1, T2>& p)
{
    return details::std_pair_wrapper<T1, T2>(p);
}

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__STD_PAIR_HH__
