/**
 * \file
 *
 * \brief Escape sequence codes for terminal (definition)
 *
 * \date Fri Oct 22 05:29:54 MSD 2010 -- Initial design
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
#include <zencxx/os/escape_sequences.hh>

// Standard includes

namespace zencxx { namespace os { namespace esc {

const char* const bold::on           = "\033[1m";
const char* const bold::off          = "\033[22m";

const char* const underscore::on     = "\033[4m";
const char* const underscore::off    = "\033[24m";

const char* const blink::on          = "\033[5m";
const char* const blink::off         = "\033[25m";

const char* const reverse::on        = "\033[7m";
const char* const reverse::off       = "\033[27m";

const char* const reset              = "\033[0m";

const char* const fg::black          = "\033[30m";
const char* const fg::red            = "\033[31m";
const char* const fg::green          = "\033[32m";
const char* const fg::brown          = "\033[33m";
const char* const fg::blue           = "\033[34m";
const char* const fg::magenta        = "\033[35m";
const char* const fg::cyan           = "\033[36m";
const char* const fg::white          = "\033[37m";
const char* const fg::unormal        = "\033[38m";
const char* const fg::normal         = "\033[39m";
const char* const fg::gray           = "\033[30;1m";
const char* const fg::light_red      = "\033[31;1m";
const char* const fg::light_green    = "\033[32;1m";
const char* const fg::yellow         = "\033[33;1m";
const char* const fg::light_blue     = "\033[34;1m";
const char* const fg::light_magenta  = "\033[35;1m";
const char* const fg::light_cyan     = "\033[36;1m";
const char* const fg::bright_white   = "\033[37;1m";

const char* const bg::black          = "\033[40m";
const char* const bg::red            = "\033[41m";
const char* const bg::green          = "\033[42m";
const char* const bg::brown          = "\033[43m";
const char* const bg::blue           = "\033[44m";
const char* const bg::magenta        = "\033[45m";
const char* const bg::cyan           = "\033[46m";
const char* const bg::white          = "\033[47m";
const char* const bg::normal         = "\033[49m";

}}}                                                         // namespace esc, os, zencxx
