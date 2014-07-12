/**
 * \file
 *
 * \brief Terminal utility functions (implementation)
 *
 * \date Sun Oct  2 00:12:48 MSK 2011 -- Initial design
 * \date Mon Apr  7 14:23:05 MSK 2014 -- Add a color support detector via termcap DB (curses library)
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
#ifdef __linux__
# include <sys/ioctl.h>
#endif                                                      // __linux__
#ifdef ZENCXX_USE_CURSES
# include <term.h>
#endif                                                      // ZENCXX_USE_CURSES
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace zencxx { namespace os { namespace term { namespace {
constexpr auto RGB_COLORS_COUNT = 256 * 256 * 256;

#ifdef ZENCXX_USE_CURSES
/// \internal Helper function to get number of terminal colors
int get_supported_colors_count()
{
    static const char* const TERM = std::getenv("TERM");

    // ATTENTION Hardcoded known to be quite enough buffer size ;-)
    char term_buffer[2048];

    const auto success = tgetent(term_buffer, TERM);
    const auto result = 0;
    switch (success)
    {
        case -1:
        case 0:
            break;
        case 1:
        {
            // Get number of color available
            const auto nc = tgetnum("Co");
            if (nc != -1)
            {
                /// \attention Workaround for `konsole` terminal: it is known
                /// that latter is capable to support true color. So, check
                /// \c TERM and try to recognize "well known" value.
                if (std::strcmp(TERM, "konsole-256color") == 0)
                    return RGB_COLORS_COUNT;
                return nc;
            }
            break;
        }
        default:
            assert(!"Unexpected result value from tgetent()");
            break;
    }
    return result;
}
#endif                                                      // ZENCXX_USE_CURSES

/// \internal Helper function to get \c color enum value by number of colors obtained from termcap DB.
color num_to_color_transform(const int number_of_colors)
{
    switch (number_of_colors)
    {
        case 0:
            return color::none;
        case 8:
            return color::basic_8_color;
        case 256:
            return color::indexed_256_color;
        case RGB_COLORS_COUNT:
            return color::true_color;
        default:
            assert(!"Unexpected result value of number of colors");
            break;
    }
    return color::unknown;
}
}                                                           // anonymous namespace
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
#ifdef __linux__
    winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
        ZENCXX_THROW(os::exception("ioctl")) << zencxx::exception::reason("Unable to get terminal size");
    return std::make_pair(w.ws_col, w.ws_row);
#else                                                       // __linux__
    /// \todo Any better idea for non Linux platforms? (Windows?)
    return std::make_pair(80, 25);
#endif                                                      // !__linux__
}

/**
 * Use \c curses library to get terminal capabilities
 */
color get_term_color_capability()
{
#ifdef ZENCXX_USE_CURSES
    static const auto color_caps = num_to_color_transform(get_supported_colors_count());
    return color_caps;
#else
    /// \todo Implementation w/o using \e curses required
    ///  * iterate over hardcoded list of terminal ID string for linux
    ///  * smth else for other platforms...
    return color::unknown;
#endif                                                      // USE_CURSES
}

/**
 * \brief Check if terminal supports colors
 */
bool is_color_term()
{
    const auto color_caps = get_term_color_capability();
    return color_caps != color::unknown && color_caps != color::none;
}

}}}                                                         // namespace term, os, zencxx
