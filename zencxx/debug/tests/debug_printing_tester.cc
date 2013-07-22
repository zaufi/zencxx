/**
 * \file
 *
 * \brief Class tester for \c debug_printing
 *
 * \date Sat Jul 20 07:30:00 MSK 2013 -- Initial design
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
#if 0
#define ZENCXX_DEBUG_PRINT_NO_GENERIC
#endif
#include <zencxx/debug/print/any.hh>

// Standard includes
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <chrono>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace print = zencxx::debug::print;

namespace {
struct nonstreamable {};
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(std_chrono_system_time_point_debug_printing_test)
{
    {
        auto now = std::chrono::system_clock::now();
        std::cout << print::any(now) << std::endl;
    }
    {
        auto now = std::chrono::system_clock::now();
        std::cout << print::localtime << print::any(now) << std::endl;
    }
    {
        auto now = std::chrono::system_clock::now();
        std::cout << print::gmtime << print::any(now) << std::endl;
    }
    // Test for type w/o operator<< defined
    {
#ifndef ZENCXX_DEBUG_PRINT_NO_GENERIC
        std::cout << print::any(nonstreamable()) << std::endl;
#endif
    }
}

BOOST_AUTO_TEST_CASE(std_chrono_system_duration_debug_printing_test)
{
    {
        std::chrono::system_clock::duration d = std::chrono::seconds(0);
        std::cout << print::show_type_info << print::any(d) << print::no_show_type_info << std::endl;
    }
    {
        auto d = std::chrono::seconds(1);
        std::cout << print::any(d) << std::endl;
    }
    {
        auto d = std::chrono::minutes(1);
        std::cout << print::any(d) << std::endl;
    }
    {
        auto d = std::chrono::hours(1);
        std::cout << print::any(d) << std::endl;
    }
    {
        auto d = std::chrono::hours(1) + std::chrono::minutes(2) + std::chrono::seconds(3);
        std::cout << print::any(d) << std::endl;
    }
    {
        auto d = std::chrono::hours(25) + std::chrono::minutes(2) + std::chrono::seconds(3);
        std::cout << print::any(d) << std::endl;
    }
    {
        auto d = std::chrono::hours(59) + std::chrono::minutes(35) + std::chrono::seconds(46);
        std::cout << print::any(d) << std::endl;
    }
}
