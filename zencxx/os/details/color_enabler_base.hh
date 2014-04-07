/**
 * \file
 *
 * \brief Class \c zencxx::os::details::color_enabler_base
 *
 * \date Mon Apr  7 19:30:41 MSK 2014 -- Initial design
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

#pragma once

// Project specific includes
#include <zencxx/os/details/export.hh>
#include <zencxx/os/term/utils.hh>

// Standard includes

namespace zencxx { namespace os { namespace details {

/**
 * \brief Mixin class to inherit a way to disable/enable colors
 * into a particular manupulator-like \c color class.
 */
class ZENCXXOS_EXPORT color_enabler_base
{
public:
    static bool is_enabled()
    {
        return *get_flag_location();
    }
    static void set_enable(const bool flag)
    {
        *get_flag_location() = flag;
    }

private:
    static bool* get_flag_location()
    {
        static bool s_is_color_enabled;
        return &s_is_color_enabled;
    }
};

}}}                                                         // namespace details, os, zencxx
