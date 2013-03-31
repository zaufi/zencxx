/**
 * \file
 *
 * \brief Class \c zencxx::os::daemon_application (interface)
 *
 * \date Sat Mar 23 08:59:53 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__OS__DAEMON_APPLICATION_HH__
# define __ZENCXX__OS__DAEMON_APPLICATION_HH__

// Project specific includes
# include <zencxx/os/exception.hh>
# include <zencxx/os/details/export.hh>

// Standard includes

namespace zencxx { namespace os {

/**
 * \brief Class to help application to become a correct daemon
 *
 * All that you need is to inherit and call \c daemonize()
 * method in your application class.
 *
 * \todo Add a PID file name to constructor (as \c boost::filesystem::path)
 * and work w/ it on \c daemonize().
 *
 */
class ZENCXXOS_EXPORT daemon_application
{
public:
    struct exception : virtual public zencxx::exception
    {
        struct daemionization_failure;
    };
    bool daemonize();                                       ///< Become a daemon
};

struct ZENCXXOS_EXPORT daemon_application::exception::daemionization_failure
  : public daemon_application::exception
  , public os::exception
{
    explicit daemionization_failure(const char* const api_function_name)
      : os::exception(api_function_name)
    {}
};

}}                                                          // namespace os, zencxx
#endif                                                      // __ZENCXX__OS__DAEMON_APPLICATION_HH__
