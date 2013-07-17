/**
 * \file
 *
 * \brief Class tester for \c unilock
 *
 * \date Wed Jul 17 13:44:42 MSK 2013 -- Initial design
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
#include <zencxx/thread/unilock.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
// #define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <boost/thread.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

using namespace zencxx;

namespace {
const int MAX_THREADS = 3;
struct fuxture_1
{
    void locker(const int id)
    {
        std::cout << id << ") Going to read lock..." << std::endl;
        m_lock.lock(exclusive_lock::lock);
        std::cout << id << ") Locked! Going to sleep..." << std::endl;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        m_lock.unlock(exclusive_lock::lock);
        std::cout << id << ") Unlocked! Exiting..." << std::endl;
        m_count++;
        if (m_count == MAX_THREADS)
            m_promise.set_value(true);
    }
    unilock<default_scheduler<exclusive_lock>> m_lock;
    boost::promise<bool> m_promise;
    int m_count = 0;
};

}                                                           // anonymous namespace

// Single thread, default scheduler test
BOOST_AUTO_TEST_CASE(st_ds_unilock_test)
{
    unilock<default_scheduler<exclusive_lock>> l;
    l.lock(exclusive_lock::lock);
    BOOST_CHECK(!l.try_lock(exclusive_lock::lock));
    l.unlock(exclusive_lock::lock);
    BOOST_CHECK(l.try_lock(exclusive_lock::lock));
    l.unlock(exclusive_lock::lock);
}

// Multiple thread, default scheduler test
BOOST_FIXTURE_TEST_CASE(mt_ds_unilock_test, fuxture_1)
{
    boost::thread_group g;
    for (int i = 0; i < MAX_THREADS; ++i)
        g.create_thread(std::bind(&fuxture_1::locker, this, i));

    BOOST_CHECK(m_promise.get_future().get());
    BOOST_CHECK_EQUAL(m_count, MAX_THREADS);
}
