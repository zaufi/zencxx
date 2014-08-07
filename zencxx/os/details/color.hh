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

struct indexed_rgb_tag;
struct true_color_tag;

/// \internal Generic template is undefined.
/// Only specializations have meaning.
template <typename TypeTag, typename ColorSpaceTag>
constexpr const char* get_initial_seq();

/// \internal Returns initial ESC sequence for foreground RGB color space
template <>
inline constexpr const char* get_initial_seq<foreground_tag, indexed_rgb_tag>()
{
    return "\033[38;5;";
}

/// \internal Returns initial ESC sequence for foreground 16M color space
template <>
inline constexpr const char* get_initial_seq<foreground_tag, true_color_tag>()
{
    return "\033[38;2;";
}

/// \internal Returns initial ESC sequence for background RGB color space
template <>
inline constexpr const char* get_initial_seq<background_tag, indexed_rgb_tag>()
{
    return "\033[48;5;";
}

/// \internal Returns initial ESC sequence for background 16M color space
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
     * Construct from indices of 256x256x256 color cube
     */
    rgb(const int r, const int g, const int b, const bool reset_required = true)
      : color_reset_base{reset_required}
      , r_{validate_component_index(r)}
      , g_{validate_component_index(g)}
      , b_{validate_component_index(b)}
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
    static constexpr std::uint8_t validate_component_index(const int idx)
    {
        return idx < END_INDEX
          ? idx
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid RGB color index")
          ;
    }

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
     */
    constexpr indexed_rgb(const int r, const int g, const int b, const bool reset_required = true)
      : color_reset_base{reset_required}
      , r_{validate_component_index(r)}
      , g_{validate_component_index(g)}
      , b_{validate_component_index(b)}
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
            os << get_initial_seq<TypeTag, indexed_rgb_tag>()
               << c.components_to_index()
               << 'm'
               ;
        }
        return os;
    }

private:
    static constexpr std::uint8_t validate_component_index(const int idx)
    {
        return idx < END_INDEX
          ? idx
          : ZENCXX_THROW(zencxx::exception()) << exception::reason("Invalid indexed RGB color index")
          ;
    }
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
     * Construct from gray scale color index, which is a value in the range <tt>[0, 23]</tt>.
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
            os << get_initial_seq<TypeTag, indexed_rgb_tag>()
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
