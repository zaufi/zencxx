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

// Standard includes
# include <iosfwd>

namespace zencxx { namespace debug { namespace print { namespace details {

template <typename T>
class value_store_wrapper
{
    const T m_value;
public:
    typedef T wrapped_type;
    explicit value_store_wrapper(const T& r) : m_value(r) {}
    T ref() const
    {
        return m_value;
    }
};

/// Print (wrapped) chars in a fancy way
std::ostream& operator<<(std::ostream& os, const value_store_wrapper<char> vw);
}                                                           // namespace details

inline details::value_store_wrapper<char> any(const char c)
{
    return details::value_store_wrapper<char>(c);
}

}}}                                                         // namespace print, debug, zencxx
#endif                                                      // __ZENCXX__DEBUG__PRINT__BUILTINS_HH__
