/**
 * \file
 *
 * \brief Class tester for \c random
 *
 * \date Sun Jun 30 21:05:25 MSK 2013 -- Initial design
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
#include <zencxx/utility/random.hh>
#include <zencxx/debug/dump_memory.hh>

// Standard includes
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>
#include <vector>

// Uncomment if u want to use boost test output streams.
// Then just output smth to it and validate an output by
// BOOST_CHECK(out_stream.is_equal("Test text"))
// using boost::test_tools::output_test_stream;

using namespace zencxx;

namespace {

constexpr std::size_t MAX_SIZE = 300;
constexpr unsigned MAX_ITERATIONS = 100;

template <typename Range>
void test_alphabet(const Range& range)
{
    for (auto i = 0; i < MAX_ITERATIONS; ++i)
    {
        auto result = make_random_string(MAX_SIZE, range);
        BOOST_CHECK_EQUAL(result.size(), MAX_SIZE);
        //
        auto pos = 0;
        for (auto ch : result)
        {
            if (std::find(std::begin(range), std::end(range), ch) == std::end(range))
            {
                BOOST_FAIL(
                    "Unexpected character '" << ch << "' at position " << pos <<
                    ". Alphabet: '" << range << "'. Result:\n" << debug::dump_memory(result)
                  );
            }
            pos++;
        }
    }
}
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(random_test)
{
    test_alphabet(alphabet::DIGITS);
    test_alphabet(alphabet::XDIGITS);
    test_alphabet(alphabet::UPPERCASE_LETTERS);
    test_alphabet(alphabet::LOWERCASE_LETTERS);
    test_alphabet(alphabet::ALPHANUMERIC);
    test_alphabet(alphabet::ALL_PRINTABLE_SYMBOLS);
    test_alphabet(std::string("qwertyuiopasdfghjklzxcvbnm"));
}

BOOST_AUTO_TEST_CASE(random_test_2)
{
    {
        auto a = peek_random_number_from_range(0, 10);
        std::cout << "a=" << a << std::endl;
    }
    {
        std::list<std::string> l = {"uno", "dos", "tres", "quatro"};
        auto a = peek_random_item_from_range(begin(l), end(l));
        std::cout << "a=" << a << std::endl;
    }
    {
        auto a = peek_random_item_from_range(boost::make_iterator_range("HelloAfrica!"));
        std::cout << "a=" << a << std::endl;
    }
    {
        auto a = peek_random_item_from_range(std::string("HelloAfrica!"));
        std::cout << "a=" << a << std::endl;
    }
    {
        auto a = peek_random_item_from_range(std::vector<int>{1, 2, 3, 4});
        std::cout << "a=" << a << std::endl;
    }
    {
        std::vector<std::string> l = {"uno", "dos", "tres", "quatro"};
        auto a = peek_random_item_from_range(l);
        std::cout << "a=" << a << std::endl;
    }
}
