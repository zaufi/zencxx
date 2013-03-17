/**
 * \file
 *
 * \brief Class tester for \c exception
 *
 * \date Thu Mar 14 06:47:35 MSK 2013 -- Initial design
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
#include <zencxx/exception.hh>
// Debugging helpers from zencxx::debug namespace
#include <zencxx/debug/type_name.hh>

// Standard includes
#include <boost/format.hpp>
// ALERT The following #define must be enabled only in one translation unit
// per unit test binary (which may consists of several such modules)
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
// Include the following file if u need to validate some text results
// #include <boost/test/output_test_stream.hpp>
#include <iostream>

namespace dbg = zencxx::debug;

namespace my {
struct exception_group : public zencxx::exception
{
    struct try_again;
    struct invalid_command;
};

struct exception_group::try_again : public exception_group {};
struct exception_group::invalid_command : public exception_group {};

struct other_exception : public zencxx::exception {};

}                                                           // namespace my

namespace {

void throw_try_again()
{
    ZENCXX_THROW(my::exception_group::try_again())
      << zencxx::exception::reason("Resource busy! Try again later...");
}

void throw_invalid_command(const char* const command)
{
    ZENCXX_THROW(my::exception_group::invalid_command())
      << zencxx::exception::reason(boost::format("Invalid command: %1%") % command);
}

void dump_exception(const zencxx::exception& e)
{
    std::cout << "Original exception type: " << e.type_info() << "\n"
        "Reason message: " << e.reason() << "\n"
        "Location: " << e.where() << "\n"
        "Backtrace:\n" << e.trace();
}

template <typename E>
void handle_exception()
{
    try
    {
        throw;
    }
    catch (const E& e)
    {
        std::cout << "** HANDLE PARTICULAR EXCEPTION **" << std::endl;
        BOOST_CHECK_EQUAL(e.type_info(), zencxx::debug::type_name<E>());
        dump_exception(e);
    }
    catch (...)
    {
        BOOST_FAIL("Unexpected exception");
    }
}

void handle_exception_group()
{
    try
    {
        throw;
    }
    catch (const my::exception_group& e)
    {
        std::cout << "** HANDLE AS EXCEPTION GROUP **" << std::endl;
        dump_exception(e);
    }
    catch (...)
    {
        BOOST_FAIL("Unexpected exception");
    }
}

void handle_base_exception()
{
    try
    {
        throw;
    }
    catch (const zencxx::exception& e)
    {
        std::cout << "** HANDLE AS BASE EXCEPTION **" << std::endl;
        dump_exception(e);
    }
    catch (...)
    {
        BOOST_FAIL("Unexpected exception");
    }
}

}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(exception_test_1)
{
    try
    {
        throw_try_again();
    }
    catch (...)
    {
        handle_exception<my::exception_group::try_again>();
    }
}

BOOST_AUTO_TEST_CASE(exception_test_2)
{
    try
    {
        throw_invalid_command("send-message");
    }
    catch (...)
    {
        handle_exception<my::exception_group::invalid_command>();
    }
}

BOOST_AUTO_TEST_CASE(exception_test_3)
{
    try
    {
        throw_try_again();
    }
    catch (...)
    {
        handle_exception_group();
    }
}

BOOST_AUTO_TEST_CASE(exception_test_4)
{
    try
    {
        throw_invalid_command("receive-message");
    }
    catch (...)
    {
        handle_base_exception();
    }
}

namespace {
void throw_nested_exception_1()
{
    try
    {
        throw_try_again();
    }
    catch (const my::exception_group&)
    {
        ZENCXX_THROW(my::other_exception())
          << zencxx::exception::reason("Some other exception")
          << zencxx::exception::current_exception()
          ;
    }
}
}                                                           // anonymous namespace

BOOST_AUTO_TEST_CASE(exception_test_5)
{
    try
    {
        throw_nested_exception_1();
    }
    catch (const my::other_exception& e)
    {
        BOOST_REQUIRE_EQUAL(e.has_nested_exception(), true);
        try
        {
            e.rethrow_nested_exception();
        }
        catch (const my::exception_group::try_again& ne)
        {
            BOOST_TEST_PASSPOINT();
        }
        catch (...)
        {
            BOOST_FAIL("Unexpected exception");
        }
    }
    catch (...)
    {
        BOOST_FAIL("Unexpected exception");
    }
}
