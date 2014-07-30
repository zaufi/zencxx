/**
 * \file
 *
 * \brief Helper classes to output 256 and 16M colors using manipulator-like way
 *
 * \date Mon Apr  7 18:13:24 MSK 2014 -- Initial design
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
#include <zencxx/os/details/color_enabler_base.hh>
#include <zencxx/os/details/color_reset_base.hh>
#include <zencxx/exception.hh>

// Standard includes
#include <cstdint>
#include <ostream>

namespace zencxx { namespace os { namespace details {

struct foreground_tag;
struct background_tag;

struct rgb_tag;
struct true_color_tag;

template <typename TypeTag, typename ColorSpaceTag>
inline constexpr const char* get_initial_seq();

template <>
inline constexpr const char* get_initial_seq<foreground_tag, rgb_tag>()
{
    return "\033[38;5;";
}

template <>
inline constexpr const char* get_initial_seq<foreground_tag, true_color_tag>()
{
    return "\033[38;2;";
}

template <>
inline constexpr const char* get_initial_seq<background_tag, rgb_tag>()
{
    return "\033[48;5;";
}

template <>
inline constexpr const char* get_initial_seq<background_tag, true_color_tag>()
{
    return "\033[48;2;";
}

/**
 * \brief Class to produce true color ESC sequences
 */
template <typename TypeTag>
class rgb
  : protected color_enabler_base
  , public color_reset_base
{
public:
    static constexpr auto END_INDEX = 256;

    /**
     * Construct from indeces of 256x256x256 color cube
     * \todo Wait for C++14 (gcc 4.9), then refactor it!
     */
    rgb(const int r, const int g, const int b, const bool reset_required = true)
      : color_reset_base{reset_required}
      , r_{
            r < END_INDEX
          ? r
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid RGB color index")
        }
      , g_{
            g < END_INDEX
          ? g
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid RGB color index")
          }
      , b_{
            b < END_INDEX
          ? b
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid RGB color index")
        }
    {
    }
    /**
     * Produce ESC sequence string for given color
     */
    friend std::ostream& operator<<(std::ostream& os, const rgb& c)
    {
        if (color_enabler_base::is_enabled())
        {
            c.reset_if_needed(os);
            os << get_initial_seq<TypeTag, true_color_tag>()
            << int(c.r_) << ';'
            << int(c.g_) << ';'
            << int(c.b_) << 'm'
            ;
        }
        return os;
    }

private:
    const std::uint8_t r_;
    const std::uint8_t g_;
    const std::uint8_t b_;
};

/**
 * \brief Class to produce 256 colors ESC sequences
 */
template <typename TypeTag>
class indexed_rgb
  : protected color_enabler_base
  , public color_reset_base
{
public:
    static constexpr auto END_INDEX = 6;

    /**
     * Construct from indices of 6x6x6 color cube
     * \todo Wait for C++14 (gcc 4.9), then refactor it!
     */
    constexpr indexed_rgb(const int r, const int g, const int b, const bool reset_required = true)
      : color_reset_base{reset_required}
      , r_{
            r < END_INDEX
          ? r
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid indexed RGB color index")
        }
      , g_{
            g < END_INDEX
          ? g
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid indexed RGB color index")
        }
      , b_{
            b < END_INDEX
          ? b
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid indexed RGB color index")
        }
    {
    }
    /**
     * Produce ESC sequence string for given color
     */
    friend std::ostream& operator<<(std::ostream& os, const indexed_rgb& c)
    {
        if (color_enabler_base::is_enabled())
        {
            c.reset_if_needed(os);
            os << get_initial_seq<TypeTag, rgb_tag>()
               << c.components_to_index()
               << 'm'
               ;
        }
        return os;
    }

private:
    /**
     * \internal Get RGB index from components according formula
     * \todo Provide refs to docs
     */
    constexpr int components_to_index() const
    {
        return r_ * 36 + g_ * 6 + b_ + 16;
    }

    const std::uint8_t r_;
    const std::uint8_t g_;
    const std::uint8_t b_;
};

/**
 * \brief Class to produce gray scale ESC sequences supported by 256 color terminals.
 */
template <typename TypeTag>
class grayscale
  : protected color_enabler_base
  , public color_reset_base
{
public:
    static constexpr auto END_INDEX = 24;

    /**
     * Construct from gray scale color index, which is a value in the range <tt>[0, 24]</tt>.
     */
    constexpr explicit grayscale(const int index, const bool reset_required = true)
      : color_reset_base{reset_required}
      , m_index{
            index < END_INDEX
          ? index
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid gray scale color index")
        }
    {
    }
    /**
     * Produce ESC sequence string for given color
     */
    friend std::ostream& operator<<(std::ostream& os, const grayscale& c)
    {
        if (color_enabler_base::is_enabled())
        {
            c.reset_if_needed(os);
            os << get_initial_seq<TypeTag, rgb_tag>()
               << c.transform_index()
               << 'm'
               ;
        }
        return os;
    }

private:
    /**
     * \internal Get color index according formula
     * \todo Provide refs to docs
     */
    constexpr int transform_index() const
    {
        return 232 + m_index;
    }
    const int m_index;
};

}}}                                                         // namespace details, os, zencxx
