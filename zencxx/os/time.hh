/**
 * \file
 *
 * \brief Cross-platform replacements for time related functions
 *
 * \date Fri Apr 11 16:48:21 MSK 2014 -- Initial design
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

// Standard includes
#include <ctime>
#include <utility>

namespace zencxx { namespace os {

/// MT-safe and cross-platform version of \c std::gmtime
inline std::pair<bool, std::tm> gmtime(const std::time_t timep)
{
    std::tm tm;
#ifdef WIN32
    auto r = gmtime_s(&tm, &timep);
    const auto result = (r == 0);
#else                                                       // WIN32
    std::tm* const r = gmtime_r(&timep, &tm);
    const auto result = r != nullptr;
#endif                                                      // !WIN32
    return std::make_pair(result, tm);
}

/// MT-safe and cross-platform version of \c std::localtime
inline std::pair<bool, std::tm> localtime(const std::time_t timep)
{
    std::tm tm;
#ifdef WIN32
    auto r = localtime_s(&tm, &timep);
    const auto result = (r == 0);
#else                                                       // WIN32
    std::tm* const r = localtime_r(&timep, &tm);
    const auto result = r != nullptr;
#endif                                                      // !WIN32
    return std::make_pair(result, tm);
}

}}                                                          // namespace os, zencxx
