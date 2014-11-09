/**
 * \file
 *
 * \brief Class \c zencxx::thread::priority_queue_adaptor (extern templates)
 *
 * \date Fri Aug 23 07:55:10 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
 * This is free software. It is licensed for use, modification and
 * redistribution under the terms of the GNU Lesser General Public License,
 * version 3 or later <http://gnu.org/licenses/lgpl.html>
 *
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
#include <zencxx/thread/et/default_scheduler.hh>
#include <zencxx/thread/predefined_lock_types.hh>
#include <zencxx/thread/priority_queue_adaptor.hh>

// Standard includes

namespace zencxx { namespace thread {

//BEGIN Explicit instantiation of adaptor w/ default_scheduler and exclusive_lock policy
extern template class priority_queue_adaptor<default_scheduler<exclusive_lock>>;

extern template void priority_queue_adaptor<
    default_scheduler<exclusive_lock>
  >::unassign_request_id(int, int);

extern template void priority_queue_adaptor<
    default_scheduler<exclusive_lock>
  >::unassign_request_id<exclusive_lock::type>(int, int, exclusive_lock::type&&);
//END Explicit instantiation of adaptor w/ default_scheduler and exclusive_lock policy

//BEGIN Explicit instantiation of adaptor w/ default_scheduler and rw_lock policy
extern template class priority_queue_adaptor<default_scheduler<rw_lock>>;

extern template void priority_queue_adaptor<
    default_scheduler<rw_lock>
  >::unassign_request_id<rw_lock::type>(int, int, rw_lock::type&&);
//END Explicit instantiation of adaptor w/ default_scheduler and rw_lock policy

}}                                                          // namespace thread, zencxx

// Instantiate one more used type
extern template class std::multimap<int, int, std::greater<int>>;
