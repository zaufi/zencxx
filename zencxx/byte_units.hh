/**
 * \file
 *
 * \brief User defined literals for bytes measurement (mostly for fun ;-)
 *
 * \date Tue Jun 19 19:57:43 MSK 2012 -- Initial design
 */
/*
 * ZenCxx is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libzencxx is distributed in the hope that it will be useful, but
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
#include <cstddef>

#ifdef BOOST_NO_CXX11_USER_DEFINED_LITERALS
# error Need a compiler with user defined literals support to use this file
#endif                                                      // BOOST_NO_CXX11_USER_DEFINED_LITERALS

namespace zencxx { namespace details {
constexpr std::size_t pow_bytes(const std::size_t what, const unsigned d)
{
    return d ? 1024 * pow_bytes(what, d - 1) : what;
}
}}                                                          // namespace details, zencxx

/// User defined literal: bytes
constexpr std::size_t operator"" _bytes(const unsigned long long size)
{
    return size;
}
/// User defined literal: kibibytes (2^10)
constexpr std::size_t operator"" _KiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 1);
}
/// User defined literal: mebibytes (2^20)
constexpr std::size_t operator"" _MiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 2);
}
/// User defined literal: gibibytes (2^30)
constexpr std::size_t operator"" _GiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 3);
}
/// User defined literal: tebibytes (2^40)
constexpr std::size_t operator"" _TiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 4);
}
/// User defined literal: pebibytes (2^50)
constexpr std::size_t operator"" _PiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 5);
}
/// User defined literal: exbibytes (2^60)
constexpr std::size_t operator"" _EiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 6);
}
/// User defined literal: zebibytes (2^70)
constexpr std::size_t operator"" _ZiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 7);
}
/// User defined literal: yobibytes (2^80)
constexpr std::size_t operator"" _YiB(const unsigned long long size)
{
    return zencxx::details::pow_bytes(size, 8);
}
