/**
 * \file
 *
 * \brief Class \c zencxx::thread::unilock (extern templates)
 *
 * \date Fri Aug 23 07:54:37 MSK 2013 -- Initial design
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
#include <zencxx/thread/details/use_deadlock_check.hh>
#include <zencxx/thread/et/default_scheduler.hh>
#include <zencxx/thread/et/fifo_adaptor.hh>
#include <zencxx/thread/et/priority_queue_adaptor.hh>
#include <zencxx/thread/predefined_lock_types.hh>
#include <zencxx/thread/unilock.hh>

// Standard includes

namespace zencxx { inline namespace thread {

extern template class unilock<priority_queue_adaptor<default_scheduler<exclusive_lock>>>;

extern template bool unilock<
    priority_queue_adaptor<
        default_scheduler<exclusive_lock>
      >
  >::template lock_decorator<int>(
        bool(unilock<priority_queue_adaptor<default_scheduler<exclusive_lock>>>::*)(
            boost::unique_lock<boost::mutex>&
          , int
          , int&&
          )
      , int&&
      );


extern template class unilock<priority_queue_adaptor<default_scheduler<rw_lock>>>;

extern template bool unilock<
    priority_queue_adaptor<
        default_scheduler<rw_lock>
      >
  >::template lock_decorator<int, rw_lock::type>(
        bool(unilock<priority_queue_adaptor<default_scheduler<rw_lock>>>::*)(
            boost::unique_lock<boost::mutex>&
          , int
          , int&&
          , rw_lock::type&&
          )
      , int&&
      , rw_lock::type&&
      );

}}                                                          // namespace thread, zencxx
