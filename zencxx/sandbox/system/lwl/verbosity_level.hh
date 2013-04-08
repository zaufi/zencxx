/**
 * \file
 *
 * \brief Class verbosity_level (interface)
 *
 * \date Fri Nov 20 20:26:29 MSK 2009 -- Initial design
 */

#ifndef __SYSTEM__LWL__VERBOSITY_LEVEL_HH__
#  define __SYSTEM__LWL__VERBOSITY_LEVEL_HH__

// Project specific includes
#  include <config.h>

// Standard includes

namespace sys { namespace lwl {

/// Severity of log message
struct verbosity_level
{
    enum type
    {
        none
      , fatal
      , error
      , warning
      , notice
      , info
      , trace
      , debug
    };
};

typedef verbosity_level level;                              ///< Shorter name for @c verbosity_level

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__VERBOSITY_LEVEL_HH__
