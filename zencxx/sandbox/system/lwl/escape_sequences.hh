/**
 * \file
 *
 * \brief Class escape_sequences (interface)
 *
 * \date Wed Nov  4 07:10:12 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__LWL__ESCAPE_SEQUENCES_HH__
#  define __SYSTEM__LWL__ESCAPE_SEQUENCES_HH__

// Project specific includes
#  include <config.h>

// Standard includes

namespace sys { namespace esc {

struct bold
{
    static const char* const on;
    static const char* const off;
};

struct underscore
{
    static const char* const on;
    static const char* const off;
};

struct blink
{
    static const char* const on;
    static const char* const off;
};

struct reverse
{
    static const char* const on;
    static const char* const off;
};

extern const char* const reset;
/// \note This is not a real color actually (i.e. not a escape sequence).
/// This pointer has a special meaning for \c sys::lwl::color manipulator
/// which would restore previously used color.
extern const char* const prev;

struct fg
{
    static const char* const black;
    static const char* const red;
    static const char* const green;
    static const char* const brown;
    static const char* const blue;
    static const char* const magenta;
    static const char* const cyan;
    static const char* const white;
    static const char* const unormal;
    static const char* const normal;
    static const char* const gray;
    static const char* const light_red;
    static const char* const light_green;
    static const char* const yellow;
    static const char* const light_blue;
    static const char* const light_magenta;
    static const char* const light_cyan;
    static const char* const bright_white;
};

struct bg
{
    static const char* const black;
    static const char* const red;
    static const char* const green;
    static const char* const brown;
    static const char* const blue;
    static const char* const magenta;
    static const char* const cyan;
    static const char* const white;
    static const char* const normal;
};

}}                                                          // namespace esc, sys
#endif                                                      // __SYSTEM__LWL__ESCAPE_SEQUENCES_HH__
