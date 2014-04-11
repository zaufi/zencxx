/**
 * \file
 *
 * \brief Conversion helpers from char to hex and back
 *
 * \date Fri Sep 10 12:57:15 MSD 2010 -- Initial design
 * \date Tue Jun 12 07:00:28 MSK 2012 -- Make 'em constexpr aware
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

#pragma once

// Project specific includes

// Standard includes
#include <boost/config.hpp>
#include <utility>

namespace zencxx {
/**
 * \brief Convert hex digit to single char
 *
 * \attention Invalid hex chars will produce garbage!
 * So <b>DO NOT USE</b> this function to process user (untrusted) input!
 *
 * \todo How to prevent invalid chars? What to produce? Nulls??
 */
BOOST_CONSTEXPR inline char hex2char(const char c)
{
    return c >= 'A' ? (c & 0xdf) - 'A' + 10 : c - '0';
}
/**
 * \brief Hex characters to \c char type converter
 *
 * \attention Invalid hex chars will produce garbage!
 * So <b>DO NOT USE</b> this function to process user (untrusted) input!
 *
 * \todo How to prevent invalid chars? What to produce? Nulls??
 */
BOOST_CONSTEXPR inline char hex2char(const char high, const char low)
{
    return (hex2char(high) << 4) + hex2char(low);
}
/// Return hex encoded low tetrade of char
BOOST_CONSTEXPR inline char char2hex_l(const char c)
{
    return (c & 0x0f) > 9 ? (c & 0x0f) - 10 + 'A' : (c & 0x0f) + '0';
}
/// Return hex encoded high tetrade of char
BOOST_CONSTEXPR inline char char2hex_h(const char c)
{
    return char2hex_l(c >> 4);
}

/// Convert \c char to pair of hex chars
/// \attention GCC < 4.7 has not a \c constexpr \c std::pair constructors,
/// so better to avoid to use this function...
BOOST_CONSTEXPR inline std::pair<char, char> char2hex(const char c)
{
    return std::make_pair(char2hex_h(c), char2hex_l(c));
}

}                                                           // namespace zencxx
