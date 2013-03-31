/**
 * \file
 *
 * \brief Escape sequence codes for terminal (declaration)
 *
 * \date Fri Oct 22 05:29:54 MSD 2010 -- Initial design
 */
/*
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

#ifndef __ZENCXX__OS__ESCAPE_SEQUENCES_HH__
# define __ZENCXX__OS__ESCAPE_SEQUENCES_HH__

// Project specific includes
# include <zencxx/os/details/export.hh>

// Standard includes

namespace zencxx { namespace os { namespace esc {

struct ZENCXXOS_EXPORT bold
{
    static const char* const on;
    static const char* const off;
};

struct ZENCXXOS_EXPORT underscore
{
    static const char* const on;
    static const char* const off;
};

struct ZENCXXOS_EXPORT blink
{
    static const char* const on;
    static const char* const off;
};

struct ZENCXXOS_EXPORT reverse
{
    static const char* const on;
    static const char* const off;
};

extern const char* const reset;

struct ZENCXXOS_EXPORT fg
{
    static const char* const black;
    static const char* const red;
    static const char* const green;
    static const char* const brown;
    static const char* const blue;
    static const char* const magenta;
    static const char* const cyan;
    static const char* const white;
    static const char* const unormal;
    static const char* const normal;
    static const char* const gray;
    static const char* const light_red;
    static const char* const light_green;
    static const char* const yellow;
    static const char* const light_blue;
    static const char* const light_magenta;
    static const char* const light_cyan;
    static const char* const bright_white;
};

struct ZENCXXOS_EXPORT bg
{
    static const char* const black;
    static const char* const red;
    static const char* const green;
    static const char* const brown;
    static const char* const blue;
    static const char* const magenta;
    static const char* const cyan;
    static const char* const white;
    static const char* const normal;
};

}}}                                                         // namespace esc, os, zencxx
#endif                                                      // __ZENCXX__OS__ESCAPE_SEQUENCES_HH__
