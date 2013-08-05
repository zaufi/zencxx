/**
 * \file
 *
 * \brief Class \c zencxx::os::charset::convert (implementation)
 *
 * \date Mon Aug  5 14:52:09 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/os/charset/convert.hh>

// Standard includes

namespace zencxx { namespace os { namespace charset {

std::string converter::convert(const std::string& str)
{
    constexpr std::size_t MAX_BUFFER_SIZE = 4096;
    /// \warning iconv wants to have `char**` to input buffer -- WHY? Can it be modified?
    char* in_buffer = const_cast<char*>(str.data());
    std::size_t in_bytes_left = str.size();

    std::string result;
    // reserve space at least how much was in origin string
    result.reserve(str.size());
    std::size_t out_bytes_left;
    char out_buffer[MAX_BUFFER_SIZE];
    char* out_buffer_ptr;
    for (bool exit = false; !exit && in_bytes_left; )
    {
        out_buffer_ptr = out_buffer;
        out_bytes_left = MAX_BUFFER_SIZE;
        std::size_t sz = iconv(m_cd, &in_buffer, &in_bytes_left, &out_buffer_ptr, &out_bytes_left);
        if (static_cast<std::size_t>(-1) == sz)
        {
            switch (errno)
            {
                case E2BIG:
                    result += std::string(out_buffer, MAX_BUFFER_SIZE - out_bytes_left);
                    break;
                case EILSEQ:
                    ZENCXX_THROW(os::exception("iconv"))
                      << exception::reason("invalid byte sequence");
                case EINVAL:
                    ZENCXX_THROW(os::exception("iconv"))
                      << exception::reason("incomplete byte sequence");
                case EBADF:
                default:
                    // NOTE Unlikely this can happened in a well formed program
                    ZENCXX_THROW(os::exception("iconv"))
                      << exception::reason("bad use");
            }
        }
    }
    iconv(m_cd, 0, 0, &out_buffer_ptr, &out_bytes_left);
    result += std::string(out_buffer, MAX_BUFFER_SIZE - out_bytes_left);
    iconv(m_cd, 0, 0, 0, 0);                                //reset convert descriptor
    return result;
}

}}}                                                         // namespace charset, os, zencxx
