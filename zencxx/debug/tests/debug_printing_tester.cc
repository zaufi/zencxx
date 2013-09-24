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
#include <zencxx/debug/print/any.hh>
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/io/ios_state.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace debug = zencxx::debug;
namespace print = zencxx::debug::print;

namespace {
struct nonstreamable {};
struct convertible_via_member
{
    std::string to_string() const
    {
        return "convertible_via_member";
    }
};
struct convertible_via_adl {};
std::string to_string(const convertible_via_adl&)
{
    return "convertible_via_adl";
}
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(convertible_to_string_debug_printing_test)
{
    {
        std::cout << print::any(convertible_via_member()) << std::endl;
    }
    {
        std::cout << print::any(convertible_via_adl()) << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(nonstreamable_debug_printing_test)
{
    // Test for type w/o operator<< defined
    std::cout << print::any(nonstreamable()) << std::endl;
}

BOOST_AUTO_TEST_CASE(streamable_types_debug_printing_test)
{
    {
        int a = 123;
        std::cout << print::any(a) << std::endl;
    }
    {
        boost::uuids::string_generator parser;
        auto id = parser("deadc0de-face-cafe-beef-c0fe00000000");
        std::cout << print::any(id) << std::endl;
    }
    {
        auto a = 3.1415;
        std::cout << print::show_type_info << print::any(a) << std::endl;
    }
    {
        auto a = 2.73f;
        std::cout << print::any(a) << std::endl;
        print::no_show_type_info(std::cout);
    }
    {
        int a = 123;
        std::cout << print::any(&a) << std::endl;
    }
    {
        std::cout << print::any(true) << std::endl;
        print::show_type_info(std::cout);
        std::cout << print::any(false) << std::endl;
        print::no_show_type_info(std::cout);
    }
    {
        const unsigned b = 456;
        print::show_type_info(std::cout);
        std::cout << print::any(&b) << std::endl;
        print::no_show_type_info(std::cout);
    }
    {
        std::cout << print::any('H') << std::endl;
        std::cout << print::any('\n') << std::endl;
        std::cout << print::any('\xab') << std::endl;
#if 0
        std::cout << print::any(u'a') << std::endl;
#endif
    }
    {
        auto a = "Hello Africa!";
        std::cout << print::any(a) << std::endl;
        /// \todo any("Hello Africa!")
#if 0
        std::cout << print::any("Hello Australia!") << std::endl;
#endif
    }
    {
        char* tmp = strdup("Hello Australia!");
        std::cout << print::show_type_info << print::any(tmp) << std::endl;
        std::free(tmp);
        print::no_show_type_info(std::cout);
    }
    {
        void* p = nullptr;
        std::cout << print::any(p) << std::endl;
        const void* cp = nullptr;
        std::cout << print::show_type_info << print::any(cp) << std::endl;
        print::no_show_type_info(std::cout);
    }
}

BOOST_AUTO_TEST_CASE(std_pair_debug_printing_test)
{
    {
        auto a = std::make_pair(123, 'H');
        std::cout << print::any(a) << std::endl;
    }
    {
        auto a = std::make_pair(3.1415f, 2.73);
        std::cout << print::show_type_info
          << print::any(a)
          << print::no_show_type_info
          << std::endl;
    }
    {
        std::pair<unsigned int const, std::string> a = {123, std::string("Hello")};
        std::cout << print::show_type_info
          << print::any(a)
          << print::no_show_type_info
          << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(containers_debug_printing_test)
{
#if 0
    {
        int a[4] = {10, 20, 30, 40};
        std::cout << print::any(a) << std::endl;
    }
#endif
    {
        std::vector<int> a = {100, 200, 300, 400};
        std::cout << print::any(a) << std::endl;
    }
    {
        std::map<unsigned, const char*> a = {
            {1, "Jan"}
          , {2, "Feb"}
          , {3, "Mar"}
        };
        std::cout << print::any(a) << std::endl;
    }
    {
        std::unordered_set<std::string> a = {
            {"Uno"}
          , {"Dos"}
          , {"Tres"}
        };
        std::cout << print::show_type_info
          << print::any(a)
          << print::no_show_type_info
          << std::endl;
    }
    {
        std::string a = "Hello Americas!";
        std::cout << print::show_type_info
          << print::any(a)
          << print::no_show_type_info
          << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(std_tuple_debug_printing_test)
{
    {
        auto a = std::make_tuple('A', short(123), 456, 3.1415, 2.73f, "Hello Antarctida!");
        std::cout << print::any(a) << std::endl;
    }
    {
        int i = 123;
        auto a = std::make_tuple('A', std::ref(i));
        std::cout << print::show_type_info << print::any(a) << print::no_show_type_info << std::endl;
    }
}

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
        auto d = std::chrono::hours(60) + std::chrono::minutes(35) + std::chrono::seconds(46);
        std::cout << print::any(d) << std::endl;
    }
}
