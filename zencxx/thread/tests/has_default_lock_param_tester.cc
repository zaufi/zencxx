/**
 * \file
 *
 * \brief Class tester for \c has_default_lock_param
 *
 * \date Wed Aug 21 01:10:15 MSK 2013 -- Initial design
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
#include <zencxx/thread/details/has_default_lock_param.hh>
#include <zencxx/thread/predefined_lock_types.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(has_default_lock_param_test)
{
    BOOST_CHECK(!zencxx::thread::details::has_default_lock_param<int>::value);
    BOOST_CHECK(!zencxx::thread::details::has_default_lock_param<zencxx::thread::rw_lock>::value);
    BOOST_CHECK(zencxx::thread::details::has_default_lock_param<zencxx::thread::exclusive_lock>::value);
}
