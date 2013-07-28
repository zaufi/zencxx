/**
 * \file
 *
 * \brief Class tester for \c ticker
 *
 * \date Mon Mar 25 06:42:02 MSK 2013 -- Initial design
 */
/*
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
#include <zencxx/ticker.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/dump_memory.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
// #define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

namespace dbg = zencxx::debug;

namespace {
struct prepare_ticker
{
    boost::asio::io_service m_srv;
    std::shared_ptr<zencxx::ticker> m_ticker = zencxx::ticker::create(m_srv);
    bool m_done = false;
};
}                                                           // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(ticker_test, prepare_ticker)

BOOST_FIXTURE_TEST_CASE(immediate_run_test, prepare_ticker)
{
    m_ticker->call([&]() { m_done = true; });
    m_srv.run_one();
    BOOST_CHECK(m_done);
}

BOOST_FIXTURE_TEST_CASE(abs_time_run_test, prepare_ticker)
{
    auto now = std::chrono::system_clock::now();
    m_ticker->call(
        [&]()
        {
            m_done = true;
        }
      , now + std::chrono::seconds(2)
      );
    m_srv.run_one();
    BOOST_CHECK(m_done);
}

BOOST_FIXTURE_TEST_CASE(stopping_abs_time_run_test, prepare_ticker)
{
    auto now = std::chrono::system_clock::now();
    auto j = m_ticker->call(
        [&]()
        {
            m_done = true;
        }
      , now + std::chrono::seconds(2)
      );
    j.stop();
    m_srv.run_one();
    BOOST_CHECK_EQUAL(m_done, false);
    j.start();
    m_srv.reset();
    m_srv.run_one();
    BOOST_CHECK_EQUAL(m_done, true);
}

BOOST_FIXTURE_TEST_CASE(canceling_abs_time_run_test, prepare_ticker)
{
    auto now = std::chrono::system_clock::now();
    auto j = m_ticker->call(
        [&]()
        {
            m_done = true;
        }
      , now + std::chrono::seconds(2)
      );
    j.cancel();
    m_srv.run_one();
    BOOST_CHECK_EQUAL(m_done, false);
    BOOST_CHECK_NO_THROW(j.cancel());
}

BOOST_FIXTURE_TEST_CASE(abs_time_expired_run_test, prepare_ticker)
{
    auto now = std::chrono::system_clock::now();
    auto j = m_ticker->call(
        [&]()
        {
            m_done = true;
        }
      , now - std::chrono::seconds(2)
      );
    j.cancel();
    m_srv.run_one();
    BOOST_CHECK_EQUAL(m_done, false);
}

BOOST_FIXTURE_TEST_CASE(rel_time_run_test, prepare_ticker)
{
    int counter = 0;
    zencxx::ticker::job j = m_ticker->call(
        [&]()
        {
            if (++counter > 2)
            {
                j.stop();
            }
        }
      , std::chrono::seconds(1)
      );
    m_srv.run();
    BOOST_CHECK_EQUAL(counter, 3);
    j.cancel();
}

BOOST_FIXTURE_TEST_CASE(rel_time_reschedule_self_test, prepare_ticker)
{
    int counter = 0;
    zencxx::ticker::job j = m_ticker->call(
        [&]()
        {
            j.stop();
            if (++counter < 3)
            {
                j.start();
            }
        }
      , std::chrono::seconds(1)
      );
    m_srv.run();
    j.cancel();
    BOOST_CHECK_EQUAL(counter, 3);
}

BOOST_FIXTURE_TEST_CASE(dead_task_removal_test, prepare_ticker)
{
    auto now = std::chrono::system_clock::now();
    auto j = m_ticker->call(
        [&]()
        {
            m_done = true;
        }
      , now - std::chrono::seconds(2)
      );
    m_srv.run_one();
    BOOST_CHECK_EQUAL(m_done, true);
    BOOST_CHECK_NO_THROW(j.cancel());
}

BOOST_AUTO_TEST_SUITE_END()                                 // End of ticker_test suite
