/**
 * \file
 *
 * \brief Class escape_sequences (implementation)
 *
 * \date Wed Nov  4 07:10:12 MSK 2009 -- Initial design
 */

// Project specific includes
#include <config.h>
#include <system/lwl/escape_sequences.hh>

// Standard includes

namespace sys { namespace esc {

const char* const bold::on           = "\033[1m";
const char* const bold::off          = "\033[22m";

const char* const underscore::on     = "\033[4m";
const char* const underscore::off    = "\033[24m";

const char* const blink::on          = "\033[5m";
const char* const blink::off         = "\033[25m";

const char* const reverse::on        = "\033[7m";
const char* const reverse::off       = "\033[27m";

const char* const reset              = "\033[0m";
///< \attention There is no any meaningful value required! Only pointer value has meaning!
const char* const prev = "\xFA";

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

}}                                                          // namespace esc, sys
