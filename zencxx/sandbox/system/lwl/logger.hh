/**
 * \file
 *
 * \brief Class logger (interface)
 *
 * \date Wed Oct 21 01:18:48 MSD 2009 -- Initial design
 */

#ifndef __SYSTEM__LWL__LOGGER_HH__
#  define __SYSTEM__LWL__LOGGER_HH__

// Project specific includes
#  include <config.h>
#  include <system/lwl/escape_sequences.hh>
#  include <system/lwl/details/log_proxy.hh>

// Standard includes
#  include <boost/preprocessor/stringize.hpp>

#  define DUMP_VAR(v) BOOST_PP_STRINGIZE(v)" = " << v

namespace sys { namespace lwl {

using details::log;
using details::set_thread_id;

}}                                                          // namespace lwl, sys
#endif                                                      // __SYSTEM__LWL__LOGGER_HH__
