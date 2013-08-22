/**
 * \file
 *
 * \brief Class \c zencxx::thread::unilock_exception (interface)
 *
 * \date Wed Aug 21 17:43:47 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__THREAD__EXCEPTION_HH__
# define __ZENCXX__THREAD__EXCEPTION_HH__

// Project specific includes
# include <zencxx/os/exception.hh>
# include <zencxx/details/export.hh>

// Standard includes

namespace zencxx { inline namespace thread {

/**
 * \brief A group for unilock exceptions
 *
 * This group consist of particular exceptions the \c unilock class may throw.
 *
 */
ZENCXX_EXPORT struct unilock_exception : public os::exception
{
    struct deadlock;
    struct not_owner_of_lock;
    struct unilock_error;

    unilock_exception(const boost::system::error_code& code, const char* const fn)
      : os::exception(code, fn)
    {}
};

}                                                           // namespace thread

ZENCXX_EXPORT struct thread::unilock_exception::deadlock
  : public thread::unilock_exception
{
    deadlock()
      : thread::unilock_exception(
          {boost::system::errc::resource_deadlock_would_occur, boost::system::system_category()}
        , "unilock::try_lock"
        )
    {}
};

ZENCXX_EXPORT struct thread::unilock_exception::not_owner_of_lock
  : public thread::unilock_exception
{
    not_owner_of_lock()
      : thread::unilock_exception(
          {boost::system::errc::operation_not_permitted, boost::system::system_category()}
        , "unilock::unlock"
        )
    {}
};

/**
 * \brief Generic unilock error
 *
 * This is an exception used to transform any other underlaid boost exceptions
 * can happen under the hood.
 */
ZENCXX_EXPORT struct thread::unilock_exception::unilock_error
  : public thread::unilock_exception
{
    explicit unilock_error(const int code = errno)
      : unilock_error({code, boost::system::system_category()})
    {}
    unilock_error(const boost::system::error_code& code)
      : thread::unilock_exception(code, "unilock::")
    {}
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__THREAD__EXCEPTION_HH__
