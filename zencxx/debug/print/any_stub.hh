/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::details::any_stub (interface)
 *
 * \date Sun Jul 28 13:53:30 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__ANY_STUB_HH__
# define __ZENCXX__DEBUG__PRINT__ANY_STUB_HH__

// Project specific includes
# include <zencxx/debug/print/any_wrapper.hh>
# include <zencxx/debug/type_name.hh>

// Standard includes
# include <ostream>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
struct any_stub : public any_wrapper<T>
{
    using any_wrapper<T>::any_wrapper;
};

/// Make \c any streamable
template <typename T>
std::ostream& operator<<(std::ostream& os, const any_stub<T>&)
{
    os << '<' << type_name<T>() << " is not printable>";
    return os;
}
}}}}                                                        // namespace details, print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__ANY_STUB_HH__
