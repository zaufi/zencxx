/**
 * \file
 *
 * \brief Get type name as string
 *
 * \date Fri Jun 29 06:43:43 MSK 2012 -- Initial design
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

#ifndef __ZENCXX__DEBUG__TYPE_NAME_HH__
# define __ZENCXX__DEBUG__TYPE_NAME_HH__

// Project specific includes

// Standard includes
# if defined(__GNUC__) && __GNUC__ > 3
#   include <cxxabi.h>
#   include <cassert>
# endif                                                     // defined(__GNUC__) && __GNUC__ > 3
# include <string>
# include <typeinfo>

namespace zencxx { namespace debug {

/**
 * \brief Try to demangle given name
 */
inline std::string demangle_name(const char* const name)
{
    // Try to demangle name using gcc API (of couse if current compiler is gcc)
#  if defined(__GNUC__) && __GNUC__ > 3
    int status;
    char* const demangled_name = abi::__cxa_demangle(name, 0, 0, &status);
    if (!status)
    {
        std::string name_str = demangled_name;
        free(demangled_name);
        return name_str;
    }
    assert(!"How is that possible? Unable to demangle name produced by itself??");
#  endif                                                    // defined(__GNUC__) && __GNUC__ > 3
    return name;
}

/**
 * \brief Get type name as string
 */
template <typename T>
inline std::string type_name(const T&)
{
    return demangle_name(typeid(T).name());
}

}}                                                          // namespace debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__TYPE_NAME_HH__
