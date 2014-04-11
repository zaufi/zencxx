/**
 * \file
 *
 * \brief Class \c zencxx::debug::backtrace (interface)
 *
 * \date Thu Mar 14 02:58:21 MSK 2013 -- Initial design
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

#pragma once

// Project specific includes
#if defined(__linux__)
#  include <zencxx/debug/details/backtrace_impl_linux.hh>
#  define ZENCXX_BACKTRACE_IMPL_NS linux_impl
#else
#  warning "This platform have no backtrace class implementation yet!"
#  include <zencxx/debug/details/backtrace_impl_dummy.hh>
#  define ZENCXX_BACKTRACE_IMPL_NS dummy_impl
#endif                                                      // defined(__linux__)

// Standard includes

namespace zencxx { namespace debug {

// Inject implementation into debug namespace
using details::ZENCXX_BACKTRACE_IMPL_NS::backtrace;

}}                                                          // namespace debug, zencxx

