/**
 * \file
 *
 * \brief Class \c zencxx::exception (implementation)
 *
 * \date Thu Mar 14 06:30:19 MSK 2013 -- Initial design
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

// Standard includes
#include <boost/format.hpp>

namespace zencxx { namespace {
constexpr auto EMPTY = debug::location{};
}                                                           // anonymous namespace

/**
 * Get string from a given \c boost::format instance (apply format)
 */
auto exception::reason(const boost::format& fmt) -> reason_info_string
{
    return reason_info_string(fmt.str());
}

/**
 * \invariant Backtrace expected to be \b always attached!
 */
const debug::backtrace& exception::trace() const
{
    const debug::backtrace* result = get<trace_info>();
    assert("Backtrace data expected to be here" && result);
    return *result;
}

/**
 * \return If no exception type was attached an \e empty string will returns.
 */
std::string exception::origin_type_info() const
{
    if (const char* const* result = get<original_type_info>())
        return debug::demangle_name(*result);
    return std::string();
}

/**
 * \return If no reason string was attached an \e empty string will returns.
 * \todo How to avoid string allocation here? Return a range?
 */
std::string exception::reason() const
{
    // Try reason_info_string first
    if (const std::string* result = get<reason_info_string>())
        return *result;                                     // Return if string was attached
    if (const char* const* result = get<reason_info_literal>())
        return *result;                                     // Make a string from char*
    return std::string();                                   // ALERT RVO better than 'zero/empty object'
}

/**
 * \return If no location was attached reference to \e empty location will returns.
 */
const debug::location& exception::where() const
{
    if (const debug::location* result = get<location_info>())
        return *result;
    return EMPTY;
}

/**
 * \return \c true if some other exception has attached to this one
 */
bool exception::has_nested_exception() const
{
    return get<nested_exception_info>() != nullptr;
}

/**
 * Throws a nested exception if attached, otherwise calls \c std::terminate.
 */
void exception::rethrow_nested_exception() const
{
    if (auto* ptr = get<nested_exception_info>())
        std::rethrow_exception(*ptr);
    std::terminate();
}

}                                                           // namespace zencxx
