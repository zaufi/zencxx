/**
 * \file
 *
 * \brief Class tester for \c zencxx::thread::details::lock_matrix
 *
 * \date Sat Jul 13 07:07:54 MSK 2013 -- Initial design
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
#include <zencxx/thread/details/lock_matrix.hh>
#include <zencxx/thread/predefined_lock_types.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(exclusive_lock_matrix_test)
{
    zencxx::thread::details::lock_matrix<zencxx::exclusive_lock> l;
    BOOST_CHECK_EQUAL(l.is_locked(), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::exclusive_lock::lock), true);

    l.lock(zencxx::exclusive_lock::lock);
    BOOST_CHECK_EQUAL(l.is_locked(), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::exclusive_lock::lock), false);

    l.unlock(zencxx::exclusive_lock::lock);
    BOOST_CHECK_EQUAL(l.is_locked(), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::exclusive_lock::lock), true);
}

BOOST_AUTO_TEST_CASE(rw_lock_matrix_test)
{
    zencxx::thread::details::lock_matrix<zencxx::rw_lock> l;
    BOOST_CHECK_EQUAL(l.is_locked(), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::read), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::write), true);

    l.lock(zencxx::rw_lock::read);
    BOOST_CHECK_EQUAL(l.is_locked(), true);
    BOOST_CHECK_EQUAL(l.is_locked(zencxx::rw_lock::read), true);
    BOOST_CHECK_EQUAL(l.is_locked(zencxx::rw_lock::write), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::read), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::write), false);

    l.unlock(zencxx::rw_lock::read);
    BOOST_CHECK_EQUAL(l.is_locked(), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::read), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::write), true);

    l.lock(zencxx::rw_lock::write);
    BOOST_CHECK_EQUAL(l.is_locked(), true);
    BOOST_CHECK_EQUAL(l.is_locked(zencxx::rw_lock::read), false);
    BOOST_CHECK_EQUAL(l.is_locked(zencxx::rw_lock::write), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::read), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::write), false);

    l.unlock(zencxx::rw_lock::write);
    BOOST_CHECK_EQUAL(l.is_locked(), false);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::read), true);
    BOOST_CHECK_EQUAL(l.can_lock(zencxx::rw_lock::write), true);
}

/// \todo One more test w/ custom matrix
