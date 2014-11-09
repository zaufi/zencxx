/**
 * \file
 *
 * \brief Class tester for \c default_scheduler
 *
 * \date Wed Jul 17 13:44:42 MSK 2013 -- Initial design
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

using namespace zencxx::thread;

// Single thread, default exclusive scheduler test
BOOST_AUTO_TEST_CASE(st_ds_unilock_test)
{
    unilock<default_scheduler<exclusive_lock>> l;
    l.lock(exclusive_lock::lock);
    BOOST_CHECK(!l.try_lock(exclusive_lock::lock));
    BOOST_CHECK_THROW(l.lock(exclusive_lock::lock), unilock_exception::deadlock);
    l.unlock(exclusive_lock::lock);
    BOOST_CHECK(l.try_lock(exclusive_lock::lock));
    l.unlock(exclusive_lock::lock);
    BOOST_CHECK_THROW(l.unlock(exclusive_lock::lock), unilock_exception::not_owner_of_lock);
    {
        decltype(l)::scoped_lock sl(l, exclusive_lock::lock);
        BOOST_CHECK(!l.try_lock(exclusive_lock::lock));
    }
}

// Single thread, default exclusive scheduler test (default lock type check)
BOOST_AUTO_TEST_CASE(st_ds_unilock_test_2)
{
    unilock<default_scheduler<exclusive_lock>> l;
    l.lock();
    BOOST_CHECK(!l.try_lock());
    l.unlock();
    BOOST_CHECK(l.try_lock());
    l.unlock();
    {
        decltype(l)::scoped_lock sl(l);
        BOOST_CHECK(!l.try_lock());
    }
}

namespace {
const int MAX_THREADS = 3;
struct fixture_1
{
    void locker(const int id)
    {
        BOOST_TEST_CHECKPOINT(id << ") Going to lock...");
        m_lock.lock(exclusive_lock::lock);
        BOOST_TEST_CHECKPOINT(id << ") Locked! Going to sleep...");
        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        m_lock.unlock(exclusive_lock::lock);
        BOOST_TEST_CHECKPOINT(id << ") Unlocked! Exiting...");
        m_count++;
        if (m_count == MAX_THREADS)
            m_promise.set_value(true);
    }
    unilock<default_scheduler<exclusive_lock>> m_lock;
    boost::promise<bool> m_promise;
    int m_count = 0;
};

}                                                           // anonymous namespace

// Multiple thread, default exclusive scheduler test
BOOST_FIXTURE_TEST_CASE(mt_ds_unilock_test, fixture_1)
{
    boost::thread_group g;
    for (int i = 0; i < MAX_THREADS; ++i)
        g.create_thread(std::bind(&fixture_1::locker, this, i));

    BOOST_CHECK(m_promise.get_future().get());
    BOOST_CHECK_EQUAL(m_count, MAX_THREADS);
    g.join_all();
}

// Single thread, default rw_lock scheduler test
BOOST_AUTO_TEST_CASE(st_ds_rw_unilock_test)
{
    unilock<default_scheduler<rw_lock>> l;
    l.lock(rw_lock::read);
    BOOST_CHECK(!l.try_lock(rw_lock::write));
    l.unlock(rw_lock::read);
    BOOST_CHECK(l.try_lock(rw_lock::write));
    BOOST_CHECK(!l.try_lock(rw_lock::read));
    l.unlock(rw_lock::write);
    {
        decltype(l)::scoped_lock sl(l, rw_lock::read);
        BOOST_CHECK(!l.try_lock(rw_lock::read));
    }
    {
        decltype(l)::scoped_lock sl(l, rw_lock::write);
        BOOST_CHECK(!l.try_lock(rw_lock::write));
    }
}

namespace {
struct fixture_2
{
    enum class status
    {
        initial
      , read_acquired_1
      , read_acquired_2
      , read_released_1
      , read_released_2
      , ready_to_write_lock
      , write_acquired
      , tried_read_lock_1
      , tried_read_lock_2
      , write_released
    };

    void sleep_a_little()
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    }
    void read_lock_holder_1()
    {
        BOOST_TEST_CHECKPOINT("RL1: Entering...");
        BOOST_CHECK(m_status == status::initial);
        m_lock.lock(rw_lock::read);
        m_status = status::read_acquired_1;
        BOOST_TEST_CHECKPOINT("RL1: Locked...");
        while (m_status != status::ready_to_write_lock)
        {
            BOOST_TEST_CHECKPOINT("RL1: Waiting WL ready...");
            sleep_a_little();
        }
        m_lock.unlock(rw_lock::read);
        m_status = status::read_released_1;
        BOOST_TEST_CHECKPOINT("RL1: Lock released...");
        while (m_status != status::write_acquired)
        {
            BOOST_TEST_CHECKPOINT("RL1: Waiting WL acquire...");
            sleep_a_little();
        }
        BOOST_TEST_CHECKPOINT("RL1: Trying read lock...");
        BOOST_CHECK(!m_lock.try_lock(rw_lock::read));
        m_status = status::tried_read_lock_1;
        BOOST_TEST_CHECKPOINT("RL1: Done! Exiting");
    }

    void read_lock_holder_2()
    {
        BOOST_TEST_CHECKPOINT("RL2: Entering...");
        while (m_status != status::read_acquired_1)
        {
            BOOST_TEST_CHECKPOINT("RL2: Waiting RL1 ready...");
            sleep_a_little();
        }
        BOOST_TEST_CHECKPOINT("RL2: Read locking 2...");
        m_lock.lock(rw_lock::read);
        m_status = status::read_acquired_2;
        BOOST_TEST_CHECKPOINT("RL2: Locked...");
        while (m_status != status::read_released_1)
        {
            BOOST_TEST_CHECKPOINT("RL2: Waiting RL1 release...");
            sleep_a_little();
        }
        m_lock.unlock(rw_lock::read);
        m_status = status::read_released_2;
        BOOST_TEST_CHECKPOINT("RL2: Lock released...");
        while (m_status != status::tried_read_lock_1)
        {
            BOOST_TEST_CHECKPOINT("RL2: Waiting RL1 finished...");
            sleep_a_little();
        }
        BOOST_TEST_CHECKPOINT("RL2: Trying read lock...");
        BOOST_CHECK(!m_lock.try_lock(rw_lock::read));
        m_status = status::tried_read_lock_2;
        BOOST_TEST_CHECKPOINT("RL2: Done! Exiting");
    }

    void write_locker()
    {
        BOOST_TEST_CHECKPOINT("WL: Entering...");
        while (m_status != status::read_acquired_2)
        {
            BOOST_TEST_CHECKPOINT("WL: Waiting RL2 gets locked...");
            sleep_a_little();
        }
        BOOST_TEST_CHECKPOINT("WL: Trying write lock...");
        BOOST_CHECK(!m_lock.try_lock(rw_lock::write));
        m_status = status::ready_to_write_lock;
        BOOST_TEST_CHECKPOINT("WL: Write locking...");
        m_lock.lock(rw_lock::write);
        m_status = status::write_acquired;
        BOOST_TEST_CHECKPOINT("WL: Locked...");
        while (m_status != status::tried_read_lock_2)
        {
            BOOST_TEST_CHECKPOINT("WL: Waiting RL2 gets released...");
            sleep_a_little();
        }
        BOOST_TEST_CHECKPOINT("WL: Write unlocking...");
        m_lock.unlock(rw_lock::write);
        m_status = status::write_released;
        BOOST_TEST_CHECKPOINT("WL: Done! Exiting...");
        m_promise.set_value(true);
    }

    unilock<default_scheduler<rw_lock>> m_lock;
    boost::promise<bool> m_promise;
    std::atomic<status> m_status = {status::initial};
};
}                                                           // anonymous namespace

// Multiple thread, default rw_lock scheduler test
BOOST_FIXTURE_TEST_CASE(mt_ds_rw_unilock_test, fixture_2)
{
    boost::thread reader_1(std::bind(&fixture_2::read_lock_holder_1, this));
    boost::thread reader_2(std::bind(&fixture_2::read_lock_holder_2, this));
    boost::thread writer(std::bind(&fixture_2::write_locker, this));
    BOOST_CHECK(m_promise.get_future().get());
    reader_1.interrupt();
    reader_2.interrupt();
    writer.interrupt();
    reader_1.join();
    reader_2.join();
    writer.join();
}
