/**
 * \file
 *
 * \brief Generic function for debug-printing of any type
 *
 * \date Thu Jul 18 08:36:40 MSK 2013 -- Initial design
 *
 * \todo Generalize to use all kind of streams (i.e. including wide)...
 * But need to wait for full C++11 support (in the standard library).
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
#include <zencxx/debug/print/any_chooser.hh>

// Standard includes

namespace zencxx { namespace debug { namespace print {

template <typename T>
typename std::enable_if<
    std::is_arithmetic<T>::value || std::is_pointer<T>::value
  , typename details::any_chooser<T>::type
  >::type any(T v)
{
    return typename details::any_chooser<T>::type(v);
}

template <typename T>
typename std::enable_if<
    !(std::is_arithmetic<T>::value || std::is_pointer<T>::value)
  , typename details::any_chooser<const T&>::type
  >::type any(const T& v)
{
    return typename details::any_chooser<const T&>::type(v);
}

}}}                                                         // namespace print, debug, zencxx
