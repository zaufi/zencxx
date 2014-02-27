/**
 * \file
 *
 * \brief Class tester for \c parameterized_adaptor
 *
 * \date Fri Aug 23 07:28:48 MSK 2013 -- Initial design
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
#include <zencxx/thread/default_scheduler.hh>
#include <zencxx/thread/parameterized_adaptor.hh>
#include <zencxx/thread/priority_queue_adaptor.hh>
#include <zencxx/thread/unilock.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <boost/thread.hpp>
#include <atomic>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

using namespace zencxx;

namespace {
constexpr auto RESOURCE_1 = 123;
constexpr auto RESOURCE_2 = 456;
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(parameterized_adaptor_test)
{
    typedef unilock<
        parameterized_adaptor<
            default_scheduler<exclusive_lock>
          , int
          >
      > lock_type;

    lock_type l;
    l.lock(RESOURCE_1);
    BOOST_CHECK(l.try_lock(RESOURCE_2));
    BOOST_CHECK_THROW(l.lock(RESOURCE_1), unilock_exception::deadlock);
    l.unlock(RESOURCE_1);
    BOOST_CHECK_THROW(l.unlock(RESOURCE_1), unilock_exception::not_owner_of_lock);
    l.unlock(RESOURCE_2);
    {
        lock_type::scoped_lock sl1(l, RESOURCE_1);
        lock_type::scoped_lock sl2(l, RESOURCE_2);
        BOOST_CHECK_THROW(l.lock(RESOURCE_1), unilock_exception::deadlock);
        BOOST_CHECK_THROW(l.lock(RESOURCE_2), unilock_exception::deadlock);
    }
    BOOST_CHECK(l.try_lock(RESOURCE_2));
    l.unlock(RESOURCE_2);
}

BOOST_AUTO_TEST_CASE(parameterized_adaptor_test_1)
{
    typedef unilock<
        parameterized_adaptor<
            default_scheduler<exclusive_lock>
          , std::string
          >
      > lock_type;

    lock_type l;
    l.lock("RESOURCE_1");
    BOOST_CHECK(l.try_lock("RESOURCE_2"));
    BOOST_CHECK_THROW(l.lock("RESOURCE_1"), unilock_exception::deadlock);
    l.unlock("RESOURCE_1");
    BOOST_CHECK_THROW(l.unlock("RESOURCE_1"), unilock_exception::not_owner_of_lock);
    l.unlock("RESOURCE_2");
    {
        lock_type::scoped_lock sl1(l, "RESOURCE_1");
        lock_type::scoped_lock sl2(l, "RESOURCE_2");
        BOOST_CHECK_THROW(l.lock("RESOURCE_1"), unilock_exception::deadlock);
        BOOST_CHECK_THROW(l.lock("RESOURCE_2"), unilock_exception::deadlock);
    }
    BOOST_CHECK(l.try_lock("RESOURCE_2"));
    l.unlock("RESOURCE_2");
}
