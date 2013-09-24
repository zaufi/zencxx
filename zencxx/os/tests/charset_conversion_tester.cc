/**
 * \file
 *
 * \brief Class tester for charset conversion
 *
 * \date Mon Aug  5 15:24:23 MSK 2013 -- Initial design
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
#include <zencxx/os/charset/convert.hh>
#include <zencxx/debug/dump_memory.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

namespace charset = zencxx::os::charset;
namespace debug = zencxx::debug;

BOOST_AUTO_TEST_CASE(charset_conversion_test)
{
    {
        std::string a = charset::convert("Привет").decode("utf-8").encode("cp1251");
        std::cout << debug::dump_memory(a) << std::endl;
        const std::string expected = {'\xCF', '\xF0', '\xE8', '\xE2', '\xE5', '\xF2'};
        BOOST_CHECK_EQUAL(a, expected);
    }
    {
        std::string in = {'\xCF', '\xF0', '\xE8', '\xE2', '\xE5', '\xF2'};
        std::string a = charset::convert(in).encode("utf-8").decode("cp1251");
        std::cout << debug::dump_memory(a) << std::endl;
        BOOST_CHECK_EQUAL(a, "Привет");
    }
}

BOOST_AUTO_TEST_CASE(fail_conversion_test)
{
    // Bad usage
    BOOST_CHECK_THROW(
        static_cast<std::string>(charset::convert("test"))
      , zencxx::os::exception
      );
    BOOST_CHECK_THROW(
        static_cast<std::string>(
            charset::convert("test").decode("NOT-EXISTED-CHARSET").encode("DOESNT-MATTER")
          )
      , zencxx::os::exception
      );
}
