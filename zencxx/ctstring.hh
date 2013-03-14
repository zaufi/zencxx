/**
 * \file
 *
 * \brief C-like string functions but operates in compile time over constants
 *
 * \date Thu May 31 22:21:57 MSK 2012 -- Initial design: add just few functions used by debug library
 *
 * \note Do not use this functions for anything else than literal strings!
 * For runtime strings it is better to use glibc provided functions!
 * (which are actually highly optimized)
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

#ifndef __ZENCXX__CTSTRING_HH__
# define __ZENCXX__CTSTRING_HH__

// Project specific includes

// Standard includes
# include <cstddef>

namespace zencxx { namespace ct { namespace details {

/// \internal Collect a given string size
constexpr inline std::size_t strlen_n(
    const char* const str
  , const std::size_t current
  )
{
    return *str ? strlen_n(str + 1, current + 1) : current;
}
/// \internal Find a character whithin a range
constexpr inline const char* find_forward(
    const char* const current
  , const char* const last
  , int c
  )
{
    return current != last
      ? *current == c
        ? current
        : find_forward(current + 1, last, c)
      : nullptr
      ;
}
/// \internal Find a character whithin a range in reverse order
constexpr inline const char* find_backward(
    const char* const current
  , const char* const last
  , int c
  )
{
    return current != last
      ? *current == c
        ? current
        : find_backward(current - 1, last, c)
      : nullptr
      ;
}
}                                                           // namespace details

/// Find a string length
/// \todo Does it really needed?
constexpr inline std::size_t strlen(const char* const str)
{
    return str ? details::strlen_n(str, 0) : 0;
}

/// Find a string length (templated version)
template <std::size_t N>
constexpr inline std::size_t strlen(const char* (&)[N])
{
    return N;
}

/// Find a character within a string
/// \todo Does it really needed?
constexpr inline const char* strchr(const char* const str, int c)
{
    return str ? details::find_forward(str, str + strlen(str), c) : nullptr;
}

/// Find a character within a string (templated version)
template <std::size_t N>
constexpr inline const char* strchr(const char (&str)[N], int c)
{
    return str ? details::find_forward(str, str + N, c) : nullptr;
}

/// Find a last occurence of a character within a string
/// \todo Does it really needed?
constexpr inline const char* strrchr(const char* const str, int c)
{
    return str ? details::find_backward(str + strlen(str), str, c) : nullptr;
}

/// Find a last occurence of a character within a string (templated version)
template <std::size_t N>
constexpr inline const char* strrchr(const char (&str)[N], int c)
{
    return str ? details::find_backward(str + N, str, c) : nullptr;
}

}}                                                          // namespace ct, zencxx
#endif                                                      // __ZENCXX__CTSTRING_HH__
