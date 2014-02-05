/**
 * \file
 *
 * \brief Some predefined lock types
 *
 * \date Thu Aug 22 08:40:48 MSK 2013 -- Initial design
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

#pragma once

// Project specific includes
#include <zencxx/thread/details/export.hh>

// Standard includes

namespace zencxx { inline namespace thread {
/**
 * \brief A matrix specification for exclusive lock.
 *
 * This matrix (1x1 size) defines the only type of lock.
 * \c unilock instantiated with this matrix and \c default_scheduler
 * becomes equivalent to an ordinal mutex.
 *
 * \sa default_scheduler
 */
struct ZENCXXTHREAD_EXPORT exclusive_lock
{
    enum type
    {
        lock
    };
    static constexpr bool matrix[1][1] = {{false}};
    static constexpr type default_lock = lock;
};

/**
 * \brief A matrix specification for read/write lock
 *
 * Allows multiple read locks.
 */
struct ZENCXXTHREAD_EXPORT rw_lock
{
    enum type
    {
        read
      , write
    };
    static constexpr bool matrix[2][2] = {                  //        Read  Write
        {true,  false}                                      // Read    1      0
      , {false, false}                                      // Write   0      0
    };
};

}}                                                          // namespace thread, zencxx
