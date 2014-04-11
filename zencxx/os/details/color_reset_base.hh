/**
 * \file
 *
 * \brief Class \c zencxx::os::details::color_reset_base (interface)
 *
 * \date Tue Apr  8 03:54:49 MSK 2014 -- Initial design
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
#include <zencxx/os/details/export.hh>
#include <zencxx/os/escape_sequences.hh>

// Standard includes
#include <boost/config.hpp>
#include <ostream>

namespace zencxx { namespace os { namespace details {

/**
 * \brief Mixin class to inherit color reset functionality
 */
class ZENCXXOS_EXPORT color_reset_base
{
    const bool m_reset_required;

public:
    /// Remember a given requirement
    BOOST_CONSTEXPR explicit color_reset_base(const bool reset_required)
      : m_reset_required(reset_required)
    {}
    /// Issue \e reset ESC sequence if required
    std::ostream& reset_if_needed(std::ostream& os) const
    {
        if (m_reset_required)
            os << esc::reset;
        return os;
    }
};

}}}                                                         // namespace details, os, zencxx
