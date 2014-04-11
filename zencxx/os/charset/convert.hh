/**
 * \file
 *
 * \brief Class \c zencxx::os::charset::convert (interface)
 *
 * \date Mon Aug  5 14:52:09 MSK 2013 -- Initial design
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
#include <zencxx/os/exception.hh>
#include <zencxx/os/details/export.hh>

// Standard includes
#include <iconv.h>
#include <cassert>
#include <string>

namespace zencxx { namespace os { namespace charset {

/**
 * \brief Simple RAII wrapper class over \c iconv found in modern glibc
 */
class ZENCXXOS_EXPORT converter
{
    iconv_t m_cd;

public:
    /// Make an instance of charset converter using codepages
    /// to convert \e from and \e to
    converter(const std::string& from, const std::string& to)
      : m_cd(iconv_open(to.c_str(), from.c_str()))
    {
        if (m_cd == reinterpret_cast<iconv_t>(-1))
            ZENCXX_THROW(os::exception("iconv_open"))
              << exception::reason("iconv init failure");
    }
    /// Cleanup iconv resources
    ~converter()
    {
        const auto r = iconv_close(m_cd);
        assert(r != -1);
    }
    /// Do a conversion
    ZENCXXOS_EXPORT std::string convert(const std::string&);
};

/**
 * \brief Syntax sugar class to convert string's charset
 *
 * It can be used almost as in Python:
 * \code
 *  std::string input = ...;
 *  std::string output = convert(input).decode("cp1251").encode("utf-8");
 * \endcode
 *
 */
class convert
{
    const std::string& m_str;
    std::string m_from;
    std::string m_to;

public:
    explicit convert(const std::string& str) : m_str(str) {}
    convert& decode(const std::string& from_enc)
    {
        m_from = from_enc;
        return *this;
    }
    convert& encode(const std::string& to_enc)
    {
        m_to = to_enc;
        return *this;
    }
    operator std::string() const
    {
        if (m_from.empty() || m_to.empty())
            ZENCXX_THROW(os::exception(EINVAL, "convert"))
              << exception::reason("Bad use of conversion helper class");

        return converter(m_from, m_to).convert(m_str);
    }
};

}}}                                                         // namespace charset, os, zencxx
