/**
 * \file
 *
 * \brief OS related utility functions (implementation)
 *
 * \date Sun Apr 17 04:41:41 MSD 2011 -- Initial design
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
#include <zencxx/os/utils.hh>
#include <zencxx/os/exception.hh>

// Standard includes
#include <pwd.h>
// #include <ctime>
#include <memory>

namespace zencxx { namespace os { namespace {
/**
 * \brief Buffer size for \c getpwnam_r
 *
 * Buffer with this size will be used by \c getpwnam_r
 * is case if \c sysconf call failed.
 */
const size_t GET_USER_INFO_EMERGENCY_BUFFER_SIZE = 1024;
}                                                           // anonymous namespace


uid_t change_effective_user(const std::string& user)
{
    uid_t uid;
    if (!user.empty())
    {
        /// \todo It would be nice to have a wrapper around users database
        /// (Iterator for example)
        passwd user_info;
        passwd* result = 0;
        long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
        if (-1 == bufsize)                                  // indeterminate
            bufsize = GET_USER_INFO_EMERGENCY_BUFFER_SIZE;  // should be more than enough

        std::unique_ptr<char[]> buf(new char[bufsize]);
        int rc = getpwnam_r(user.c_str(), &user_info, buf.get(), bufsize, &result);
        if (0 == rc)
        {
            if (result)
            {
                rc = setuid(uid = user_info.pw_uid);
                if (-1 == rc) ZENCXX_THROW(exception("setuid"))
                  << zencxx::exception::reason("Unable to change user.");
            }
            else ZENCXX_THROW(exception(ENOENT, "getpwnam_r"))
              << zencxx::exception::reason("Unable to change user.");
        }
        else ZENCXX_THROW(exception(rc, "getpwnam_r"))
          << zencxx::exception::reason("Unable to change user.");
    }
    return uid;
}

}}                                                          // namespace os, zencxx
