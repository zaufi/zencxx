/**
 * \file
 *
 * \brief Random utility functions
 *
 * \date Sun Jun 30 18:24:42 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
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
#include <zencxx/details/export.hh>

// Standard includes
#include <boost/config.hpp>
#include <boost/range/iterator_range.hpp>
#include <cassert>
#include <iterator>
#include <random>
#include <string>
#include <type_traits>

namespace zencxx { namespace alphabet { namespace details {
BOOST_CONSTEXPR_OR_CONST std::size_t DIGITS_CNT = 10;
BOOST_CONSTEXPR_OR_CONST std::size_t XDIGITS_CNT = DIGITS_CNT + 6;
BOOST_CONSTEXPR_OR_CONST std::size_t LETTERS_CNT = 26;
BOOST_CONSTEXPR_OR_CONST std::size_t OTHER_CNT = 33;
/// \internal Array of ASCII letters specially ordered
ZENCXX_EXPORT extern const char s_letters[DIGITS_CNT + 2 * LETTERS_CNT + OTHER_CNT + 1];
}                                                           // namespace details
/// Digits range
const boost::iterator_range<const char*> DIGITS = {
    details::s_letters
  , details::s_letters + details::DIGITS_CNT
  };
/// Hex disits range
const boost::iterator_range<const char*> XDIGITS = {
    details::s_letters
  , details::s_letters + details::XDIGITS_CNT
  };
/// Upper-case Latin letters range
const boost::iterator_range<const char*> UPPERCASE_LETTERS = {
    details::s_letters + details::DIGITS_CNT
  , details::s_letters + details::DIGITS_CNT + details::LETTERS_CNT
  };
/// Lower-case Latin letters range
const boost::iterator_range<const char*> LOWERCASE_LETTERS = {
    details::s_letters + details::DIGITS_CNT + details::LETTERS_CNT
  , details::s_letters + details::DIGITS_CNT + 2 * details::LETTERS_CNT
  };
/// Latin letters range (both case) + digits
const boost::iterator_range<const char*> ALPHANUMERIC = {
    details::s_letters
  , details::s_letters + details::DIGITS_CNT + 2 * details::LETTERS_CNT
  };
/// Printable (all visible) ACSII symbols
const boost::iterator_range<const char*> ALL_PRINTABLE_SYMBOLS = {
    details::s_letters
  , details::s_letters + details::DIGITS_CNT + 2 * details::LETTERS_CNT + details::OTHER_CNT
  };
}                                                           // namespace alphabet

/// Get default random engine
ZENCXX_EXPORT std::default_random_engine& default_random_engine();

/**
 * \brief Peek a random number from a given numeric range
 */
template <typename T>
inline T peek_random_number_from_range(const T first, const T last)
{
    static_assert(std::is_integral<T>::value, "T must be intergal type");
    std::uniform_int_distribution<T> distribution(first, last);
    return distribution(default_random_engine());
}

/**
 * \brief Peek a random item from a given iterators range
 *
 * \todo Check that type \c Iterator conforms to corresponding concept requirements
 *
 */
template <typename Iterator>
inline auto peek_random_item_from_range(Iterator first, Iterator last) -> decltype(*first)
{
    auto size = std::distance(first, last);
    assert("Sanity check" && 0 < size);
    std::advance(
        first
        /// \todo \c -1? Really?
      , peek_random_number_from_range(static_cast<decltype(size)>(0), size - 1)
      );
    return *first;
}

/**
 * \brief Peek a random item from a given iterators range
 * \overload peek_random_item_from_range(Iterator first, Iterator last)
 */
template <typename Range>
auto peek_random_item_from_range(const Range& range) -> decltype(*std::begin(range))
{
    return peek_random_item_from_range(std::begin(range), std::end(range));
}

/// Helper function to make a random string w/ desired length
template <typename Range>
inline std::string make_random_string(
    const std::size_t length
  , const Range& range = alphabet::ALPHANUMERIC
  )
{
    std::string result(length, char());
    std::uniform_int_distribution<decltype(length)> distribution{
        0
      , static_cast<decltype(length)>(std::distance(std::begin(range), std::end(range))) - 1
      };
    auto& re = default_random_engine();
    for (auto i = 0u; i < length; ++i)
    {
        auto it = std::begin(range);
        std::advance(it, distribution(re));
        result[i] = *it;
    }
    return result;
}

}                                                           // namespace zencxx
