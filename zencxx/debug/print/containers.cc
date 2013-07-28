/**
 * \file
 *
 * \brief Debug printer for container types
 *
 * \date Fri Jul 26 08:49:44 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/debug/print/containers.hh>

// Standard includes

namespace zencxx { namespace debug { namespace print { namespace details {

std::ostream& operator<<(std::ostream& os, const any_string<const std::string&>& sw)
{
    os << '"' << sw.data() << '"';
    // Show type info if needed
    details::show_type_info_impl<any_string<std::string>::wrapped_type>(os);
    return os;
}
}}}}                                                        // namespace details, print, debug, zencxx
