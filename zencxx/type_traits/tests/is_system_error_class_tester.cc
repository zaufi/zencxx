/**
 * \file
 *
 * \brief Class tester for \c is_system_error_class
 *
 * \date Tue Oct 15 03:42:50 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/type_traits/is_system_error_class.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(is_system_error_class_test)
{
    BOOST_CHECK_EQUAL(zencxx::is_system_error_class<int>::value, false);
    BOOST_CHECK_EQUAL(zencxx::is_system_error_class<std::ostream>::value, false);

    BOOST_CHECK_EQUAL(zencxx::is_system_error_class<std::error_code>::value, true);
    BOOST_CHECK_EQUAL(zencxx::is_system_error_class<boost::system::error_code>::value, true);
}
