/**
 * \file
 *
 * \brief Class tester for \c color
 *
 * \date Mon Apr  7 18:40:27 MSK 2014 -- Initial design
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
#include <zencxx/os/color.hh>
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

using namespace zencxx::os;

BOOST_AUTO_TEST_CASE(color_test)
{
    for (auto i = 0; i < 25; ++i)
    {
#if 1
        std::cout << color::fg::grayscale(i)
          << color::bg::grayscale(24 - i) << 'O';
#else
        std::stringstream ss;
        const auto c = color::fg::grayscale(i);
        ss << c;
        std::cout << zencxx::debug::dump_memory(ss.str()) << std::endl;
#endif
    }
    std::cout << color(esc::reset) << std::endl;
}
