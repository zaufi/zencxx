/**
 * \file
 *
 * \brief Class tester for \c fifo_adaptor
 *
 * \date Thu Aug 22 15:18:52 MSK 2013 -- Initial design
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
#include <zencxx/thread/fifo_adaptor.hh>
#include <zencxx/thread/default_scheduler.hh>
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

BOOST_AUTO_TEST_CASE(st_fs_fifo_adaptor_test)
{
    unilock<fifo_adaptor<default_scheduler<exclusive_lock>>> l;
    l.try_lock();
    BOOST_CHECK(!l.try_lock());
    BOOST_CHECK_THROW(l.lock(), unilock_exception::deadlock);
    l.unlock();
    BOOST_CHECK_THROW(l.unlock(), unilock_exception::not_owner_of_lock);
    {
        decltype(l)::scoped_lock sl(l);
        BOOST_CHECK(!l.try_lock());
    }
    BOOST_CHECK(l.try_lock());
    l.unlock();
}

BOOST_AUTO_TEST_CASE(st_fs_rw_fifo_adaptor_test)
{
    unilock<fifo_adaptor<default_scheduler<rw_lock>>> l;
    l.try_lock(rw_lock::read);
    BOOST_CHECK(!l.try_lock(rw_lock::read));
    BOOST_CHECK_THROW(l.lock(rw_lock::read), unilock_exception::deadlock);
    l.unlock(rw_lock::read);
}

namespace {
struct fixture_1
{

    void lock_requester_1()
    {
        std::cout << "L1: Entering..." << std::endl;
        m_ready_count++;
        m_lock.lock(rw_lock::read);
        m_promise.set_value(true);
        std::cout << "L1: Exiting..." << std::endl;
        // NOTE This method will not release the lock!
    }

    // try to acquire a lock w/ priority 5 (looser)
    void lock_requester_2()
    {
        std::cout << "L2: Entering..." << std::endl;
        m_ready_count++;
        try
        {
            m_lock.lock(rw_lock::write);
            m_promise.set_value(false);
            m_exit_promise.set_value(false);
            std::cout << "L2: Exiting..." << std::endl;
        }
        catch (const boost::thread_interrupted&)
        {
            m_exit_promise.set_value(true);
            std::cout << "L2: Interrupted... (as expected)" << std::endl;
        }
    }

    unilock<fifo_adaptor<default_scheduler<rw_lock>>> m_lock;
    boost::promise<bool> m_promise;
    boost::promise<bool> m_exit_promise;
    std::atomic<int> m_ready_count = {0};
};
}                                                           // anonymous namespace

BOOST_FIXTURE_TEST_CASE(mt_fs_rw_unilock_test, fixture_1)
{
    // Acquire the lock before test
    m_lock.lock(rw_lock::write);
    BOOST_CHECK(!m_lock.try_lock(rw_lock::write));
    BOOST_CHECK(!m_lock.try_lock(rw_lock::read));

    // Start first locker threads: make shure it blocks first
    boost::thread locker_1(std::bind(&fixture_1::lock_requester_1, this));
    while (m_ready_count < 1)
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    // Start second locker thread: wait for block
    boost::thread locker_2(std::bind(&fixture_1::lock_requester_2, this));
    while (m_ready_count < 2)
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // Relese the lock! Starting test...
    m_lock.unlock(rw_lock::write);

    // Get results
    BOOST_CHECK(m_promise.get_future().get());
    // NOTE The lock sill acquired!
    // Send interrupts to all threads
    locker_1.interrupt();
    locker_2.interrupt();
    locker_1.join();
    locker_2.join();
    BOOST_CHECK(m_exit_promise.get_future().get());
}
