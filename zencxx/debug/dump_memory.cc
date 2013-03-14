/**
 * \file
 *
 * \brief Class \c zencxx::debug::dump_memory (implementation)
 *
 * \date Fri Sep 10 12:43:19 MSD 2010 -- Initial design
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

// Project specific includes
#include <zencxx/debug/dump_memory.hh>
#include <zencxx/hex2char_char2hex.hh>

// Standard includes
#include <boost/io/ios_state.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <locale>
#include <string>

/// \internal
#define SEPARATORS_TUPLE \
    (' ', ' ', ' ', "  ", ' ', ' ', ' ', "  ", ' ', ' ', ' ', "  ", ' ', ' ', ' ', ' ')

/// \internal
#define CHAR_DUMP(Z, N, Text) \
    << char2hex_h(Text[N]) << char2hex_l(Text[N]) \
    << BOOST_PP_TUPLE_ELEM(16, N, SEPARATORS_TUPLE)

/// \internal
#  define ASCII_DUMP(Z, N, Text) \
    << (ct.is(std::ctype_base::print, Text[N]) ? static_cast<char>(Text[N]) : '.')

namespace zencxx { namespace debug {

std::ostream& operator<<(std::ostream& os, const dump_memory& dm)
{
    boost::io::ios_all_saver ifs(os);
    // Retrieve locale attached to stream
    const std::locale& loc = os.getloc();
    // Reset locale to avoid problems when pointers get printed
    os.imbue(std::locale("C"));

    // Get ctype facet for current locale
    const std::ctype<char>& ct = std::use_facet<std::ctype<char>>(loc);

    bool did_we_print_smth = false;
    for (size_t i = 0, last = (dm.m_size / 16) * 16; i < last; i += 16)
    {
        const char* ptr = static_cast<const char*>(dm.m_ptr) + i;
        if (did_we_print_smth) os << '\n';
        os << static_cast<const void*>(ptr)
           << ": " BOOST_PP_REPEAT(16, CHAR_DUMP, ptr)
           << "| " BOOST_PP_REPEAT(16, ASCII_DUMP, ptr);
        did_we_print_smth = true;
    }
    // dump remaining bytes
    if (dm.m_size % 16)
    {
        const char* ptr = static_cast<const char*>(dm.m_ptr) + (dm.m_size / 16) * 16;
        if (did_we_print_smth) os << '\n';
        os << static_cast<const void*>(ptr) << ": ";
        ptr = static_cast<const char*>(dm.m_ptr);
        std::string ascii;
        size_t tail_sz;
        ascii.reserve(tail_sz = (dm.m_size / 16) * 16);
        for (size_t i = tail_sz; i < dm.m_size; ++i)
        {
            os << char2hex_h(ptr[i]) << char2hex_l(ptr[i]) << ' ';
            if ((i % 4) == 3 && (i != (dm.m_size - 1))) os << ' ';
            ascii += (ct.is(std::ctype_base::print, ptr[i]) ? static_cast<char>(ptr[i]) : '.');
        }
        tail_sz = 16 - dm.m_size % 16;
        os << std::string(tail_sz * 3 + tail_sz / 4, ' ') << "| " << ascii;
    }
    return os;
}

}}                                                          // namespace debug, zencxx
#undef ASCII_DUMP
#undef CHAR_DUMP
#undef SEPARATORS_TUPLE
