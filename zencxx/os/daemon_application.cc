/**
 * \file
 *
 * \brief Class \c zencxx::os::daemon_application (implementation)
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

// Project specific includes
#include <zencxx/os/daemon_application.hh>

// Standard includes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>

namespace zencxx { namespace os {
/**
 * \return \c true in a child process, \c false in a parent
 */
bool daemon_application::daemonize()
{
    auto pid = fork();
    if (pid == -1)
        ZENCXX_THROW(exception::daemionization_failure("fork"));

    if (pid > 0)                                            // Return false to the parent
        /// \todo waitpid?
        return false;

    {
        const auto rc = setsid();                           // Set current process be a group leader
        assert("setsid() expected to be Ok" && rc != -1);
    }

    // `fork()' again so the parent, (the session group leader), can exit.
    // This means that we, as a non-session group leader, can never regain a
    // controlling terminal.
    pid = fork();
    if (pid == -1)
        ZENCXX_THROW(exception::daemionization_failure("fork"));

    if (pid > 0)                                            // Return false to the parent
        return false;

    // Redirect standard streams to /dev/null
    const auto fd = open("/dev/null", O_RDWR, 0);
    if (fd == -1)
        ZENCXX_THROW(exception::daemionization_failure("open"));

    if (dup2(fd, STDIN_FILENO) == -1 || dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1)
        ZENCXX_THROW(exception::daemionization_failure("dup2"));
    if (fd > 2)
        close(fd);

    {
        // Change current directory to root to avoid possible filesystem lock
        const auto rc = chdir("/");
        assert("Can't chdir to /" && 0 == tmp);
    }
    umask(0);                                               // Drop umask
    return true;                                            // Return true to the child
}
}}                                                          // namespace os, zencxx
