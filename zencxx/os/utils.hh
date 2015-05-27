/**
 * \file
 *
 * \brief OS related utility functions (declaration)
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

#pragma once

// Project specific includes
#include <zencxx/os/details/export.hh>

// Standard includes
#include <sys/types.h>
#include <string>

namespace zencxx { namespace os {

/**
 * \brief Get user UID by name
 */
uid_t ZENCXXOS_EXPORT get_user_uid(const std::string& user);

/**
 * \brief Function to change an effective user by UID
 *
 * \param uid UID of the user to change to
 *
 * \return UID
 *
 * \throw zencxx::os::exception in case of system API failure
 *
 */
uid_t ZENCXXOS_EXPORT change_effective_user(const uid_t uid);

/**
 * \brief Function to change an effective user by name
 *
 * \param user name of the user to change to
 *
 * \return UID of effective user ID found through users DB
 *
 * \throw zencxx::os::exception in case of system API failure
 *
 * \todo Add unit tests
 */
inline uid_t change_effective_user(const std::string& user)
{
    return change_effective_user(get_user_uid(user));
}

}}                                                          // namespace os, zencxx
