/**
 * \file
 *
 * \brief Class \c zencxx::thread::default_scheduler (extern templates)
 *
 * \date Fri Aug 23 07:54:42 MSK 2013 -- Initial design
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
#include <zencxx/thread/default_scheduler.hh>
#include <zencxx/thread/predefined_lock_types.hh>

// Standard includes

namespace zencxx { namespace thread { namespace details {

extern template class default_scheduler_impl<exclusive_lock, true>;
extern template class default_scheduler_impl<exclusive_lock, false>;
extern template class default_scheduler_impl<rw_lock, false>;

}                                                           // namespace details

extern template class default_scheduler<exclusive_lock>;
extern template class default_scheduler<rw_lock>;

}}                                                          // namespace thread, zencxx
