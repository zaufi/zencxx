/**
 * \file
 *
 * \brief Class \c zencxx::os::exception (implementation)
 *
 * \date Sat Mar 23 09:00:18 MSK 2013 -- Initial design
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
#include <zencxx/os/exception.hh>

// Standard includes
#include <boost/format.hpp>

namespace zencxx { namespace os { namespace {
const boost::format ERROR_FMT("%1% (failed function='%2%', code=%3%)");
}                                                           // anonymous namespace

/**
 * \brief Override inherited function
 *
 * Join possible attached reason info and OS error code description
 * into final reason string.
 */
std::string exception::reason() const
{
    const boost::system::error_code& code = error_code();

    std::string reason_str = zencxx::exception::reason();
    if (!reason_str.empty())
        reason_str += ". ";

    reason_str += (boost::format(ERROR_FMT) % code.message() % failed_function() % code.value()).str();
    return reason_str;
}
}}                                                          // namespace os, zencxx
