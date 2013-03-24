/**
 * \file
 *
 * \brief Class \c zencxx::os::signal::mask (implementation)
 *
 * \date Sat Nov 28 21:51:03 MSK 2009 -- Initial design
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
#include <zencxx/os/signal/mask.hh>

// Standard includes
#include <utility>

namespace zencxx { namespace os { namespace signal { namespace {
/// Copy-n-pasted from \c /usr/include/bits/signum.h (linux)
const std::pair<int, const char*> S_SIG_NAMES[31] = {
      {1, "SIGHUP"}                                         // Hangup (POSIX)
    , {2, "SIGINT"}                                         // Interrupt (ANSI)
    , {3, "SIGQUIT"}                                        // Quit (POSIX)
    , {4, "SIGILL"}                                         // Illegal instruction (ANSI)
    , {5, "SIGTRAP"}                                        // Trace trap (POSIX)
    , {6, "SIGABRT"}                                        // Abort (ANSI)
    , {7, "SIGBUS"}                                         // BUS error (4.2 BSD)
    , {8, "SIGFPE"}                                         // Floating-point exception (ANSI)
    , {9, "SIGKILL"}                                        // Kill, unblockable (POSIX)
    , {10, "SIGUSR1"}                                       // User-defined signal 1 (POSIX)
    , {11, "SIGSEGV"}                                       // Segmentation violation (ANSI)
    , {12, "SIGUSR2"}                                       // User-defined signal 2 (POSIX)
    , {13, "SIGPIPE"}                                       // Broken pipe (POSIX)
    , {14, "SIGALRM"}                                       // Alarm clock (POSIX)
    , {15, "SIGTERM"}                                       // Termination (ANSI)
    , {16, "SIGSTKFLT"}                                     // Stack fault
    , {17, "SIGCHLD"}                                       // Child status has changed (POSIX)
    , {18, "SIGCONT"}                                       // Continue (POSIX)
    , {19, "SIGSTOP"}                                       // Stop, unblockable (POSIX)
    , {20, "SIGTSTP"}                                       // Keyboard stop (POSIX)
    , {21, "SIGTTIN"}                                       // Background read from tty (POSIX)
    , {22, "SIGTTOU"}                                       // Background write to tty (POSIX)
    , {23, "SIGURG"}                                        // Urgent condition on socket (4.2 BSD)
    , {24, "SIGXCPU"}                                       // CPU limit exceeded (4.2 BSD)
    , {25, "SIGXFSZ"}                                       // File size limit exceeded (4.2 BSD)
    , {26, "SIGVTALRM"}                                     // Virtual alarm clock (4.2 BSD)
    , {27, "SIGPROF"}                                       // Profiling alarm clock (4.2 BSD)
    , {28, "SIGWINCH"}                                      // Window size change (4.3 BSD, Sun)
    , {29, "SIGIO"}                                         // I/O now possible (4.2 BSD)
    , {30, "SIGPWR"}                                        // Power failure restart (System V)
    , {31, "SIGSYS"}                                        // Bad system call
};
}                                                           // anonymous namespace

//
std::ostream& operator<<(std::ostream& os, const mask& m)
{
    // Iterate over map of signal IDs to string and print every name
    int delimiter_offset = 1;
    for (const auto& p : S_SIG_NAMES)
    {
        if (m.is_set(p.first))
        {
            os << ","[delimiter_offset] << p.second;
            delimiter_offset = 0;
        }
    }
    return os;
}

}}}                                                         // namespace signal, os, zencxx
