/**
 * \file
 *
 * \brief Class \c zencxx::thread::priority_queue_adaptor (some explicit instantiations)
 *
 * \date Thu Aug 22 10:01:04 MSK 2013 -- Initial design
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

// Project specific includes
#include <zencxx/thread/et/priority_queue_adaptor.hh>

// Standard includes

namespace zencxx { inline namespace thread {

template class priority_queue_adaptor<default_scheduler<exclusive_lock>>;

template void priority_queue_adaptor<
    default_scheduler<exclusive_lock>
  >::unassign_request_id(int, int);

template void priority_queue_adaptor<
    default_scheduler<exclusive_lock>
  >::unassign_request_id(int, int, exclusive_lock::type&&);


template class priority_queue_adaptor<default_scheduler<rw_lock>>;

template void priority_queue_adaptor<
    default_scheduler<rw_lock>
  >::unassign_request_id(int, int, rw_lock::type&&);

}}                                                          // namespace thread, zencxx

template class std::multimap<int, int, std::greater<int>>;
