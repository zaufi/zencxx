/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::builtins (implementation)
 *
 * \date Thu Jul 25 09:25:32 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/debug/print/builtins.hh>
#include <zencxx/hex2char_char2hex.hh>

// Standard includes
#include <boost/io/ios_state.hpp>

namespace zencxx { namespace debug { namespace print { namespace details {

const char* try_get_special_char_repr(const char c)
{
    switch (c)
    {
        case '\'': return R"--(\')--";
        case '\a': return R"--(\a)--";
        case '\b': return R"--(\b)--";
        case '\f': return R"--(\f)--";
        case '\n': return R"--(\n)--";
        case '\r': return R"--(\r)--";
        case '\t': return R"--(\t)--";
        case '\v': return R"--(\v)--";
        default: break;
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const any_char<char> cw)
{
    const std::locale& loc = os.getloc();
    // Get ctype facet for current locale
    const std::ctype<char>& ct = std::use_facet<std::ctype<char>>(loc);
    // Print it
    os << '\'';                                             // Open single quote
    if (ct.is(std::ctype_base::print, cw.data()))
        os << cw.data();
    else
    {
        // Show special or hex values for non printable chars
        if (auto repr = try_get_special_char_repr(cw.data()))
            os << repr;
        else
            os << '\\' << 'x' << char2hex_h(cw.data()) << char2hex_l(cw.data());
    }
    os << '\'';                                             // Close single quote
    // Show type info if needed
    details::show_type_info_impl<char>(os);
    return os;
}

/**
 * \todo Do some fancy string printing w/ escape characters and hex/oct numbers for
 * non-printable chars? What about UTF-8 strings?
 */
std::ostream& operator<<(std::ostream& os, const any_pointer<const char*> pw)
{
    if (pw.data())
    {
        os << '"' << pw.data() << '"';
    }
    else
    {
        os << "nullptr";
    }
    // Show type info if needed
    details::show_type_info_impl<const char*>(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const any_pointer<char*> pw)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        if (pw.data())
        {
            os << '@' << static_cast<void*>(pw.data())
               << " -> " << any_pointer<const char*>(const_cast<const char*>(pw.data()))
               ;
        }
        else
            os << "nullptr";
    }
    // Show type info if needed
    details::show_type_info_impl<char*>(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const any_pointer<const void*> pw)
{
    if (pw.data())
        os << '@' << pw.data();
    else
        os << "nullptr";
    // Show type info if needed
    details::show_type_info_impl<const void*>(os);
    return os;
}
std::ostream& operator<<(std::ostream& os, const any_pointer<void*> pw)
{
    if (pw.data())
        os << '@' << pw.data();
    else
        os << "nullptr";
    // Show type info if needed
    details::show_type_info_impl<void*>(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const any_bool<bool> bw)
{
    boost::io::ios_flags_saver ifs(os);
    os << std::boolalpha << bw.data();
    // Show type info if needed
    details::show_type_info_impl<bool>(os);
    return os;
}

}}}}                                                        // namespace details, print, debug, zencxx
