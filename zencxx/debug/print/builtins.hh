/**
 * \file
 *
 * \brief Debug printer for builtin types
 *
 * \date Thu Jul 25 09:25:32 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__DEBUG__PRINT__BUILTINS_HH__
# define __ZENCXX__DEBUG__PRINT__BUILTINS_HH__

// Project specific includes
# include <zencxx/details/export.hh>
# include <zencxx/debug/print/any_fwd.hh>
# include <zencxx/debug/print/any_manip.hh>
# include <zencxx/debug/print/any_wrapper.hh>
# include <zencxx/hex2char_char2hex.hh>

// Standard includes
# include <locale>
# include <ostream>
# include <type_traits>

namespace zencxx { namespace debug { namespace print { namespace details {

ZENCXX_EXPORT const char* try_get_special_char_repr(const char);

template <typename T>
struct any_char : public any_wrapper<T>
{
    using any_wrapper<T>::any_wrapper;
};

/// \todo Generic implementation needed
ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_char<char>);

template <typename T>
struct any_pointer : public any_wrapper<T>
{
    static_assert(std::is_pointer<T>::value, "T must be a pointer type");
    using any_wrapper<T>::any_wrapper;
};

ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_pointer<const char*>);
ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_pointer<char*>);
ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_pointer<const void*>);
ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_pointer<void*>);

/// Pretty print for raw pointers
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const any_pointer<T>& pw)
{
    {
        details::show_type_info_saver s(os);
        no_show_type_info(os);
        if (pw.data())
            os << '@' << pw.data() << " -> " << any(*pw.data());
        else
            os << "nullptr";
    }
    // Show type info if needed
    details::show_type_info_impl<T>(os);
    return os;
}

template <typename T>
struct any_bool : public any_wrapper<T>
{
    using any_wrapper<T>::any_wrapper;
};

ZENCXX_EXPORT std::ostream& operator<<(std::ostream&, const any_bool<bool>);

}}}}                                                        // namespace details, print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__BUILTINS_HH__
