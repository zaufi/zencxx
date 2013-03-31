/**
 * \file
 *
 * \brief Terminal utility functions (implementation)
 *
 * \date Sun Oct  2 00:12:48 MSK 2011 -- Initial design
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
#include <zencxx/os/term/utils.hh>

// Standard includes
#include <sys/ioctl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace zencxx { namespace os { namespace term {
/**
 * Use \c ioctl to get terminal dimenstions.
 *
 * \return \c std::pair with width (\c first) and heigh (\c second)
 * of the current terminal window
 *
 * \throw zencxx::os::exception if \c ioctl() call failed
 */
std::pair<unsigned, unsigned> get_term_size()
{
    winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
        ZENCXX_THROW(os::exception("ioctl")) << zencxx::exception::reason("Unable to get terminal size");
    return std::make_pair(w.ws_col, w.ws_row);
}

/**
 * \brief Check if terminal supports colors
 *
 * This is very stoopid checker! However lots of open source programs
 * check color support in this way :)) For example GNU grep :)
 * I dunno why... probably cuz real checking require to link w/ curses...
 *
 * The real checker (I think) have to use terminfo DB...
 * \code
 *  static char term_buffer[2048];
 *  int success = tgetent(term_buffer, getenv("TERM"));
 *  if (success)
 *    // see `man 5 terminfo`: "Co" == max_colors numeric value
 *    int nc = tgetnum("Co");
 *    // nc must be > 0 if term supports colours
 * \endcode
 * \todo Any better idea how to check color support for current terminal?
 */
bool is_color_term()
{
    static const char* const term = getenv("TERM");
    return term && strcmp(term, "dumb");
}

}}}                                                         // namespace term, os, zencxx
