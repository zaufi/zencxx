/**
 * \file
 *
 * \brief Class \c zencxx::thread::default_scheduler (some explicit instantiations)
 *
 * \date Thu Aug 22 07:49:34 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2013 Alex Turbov and contributors, all rights reserved.
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
#include <zencxx/thread/default_scheduler.hh>

// Standard includes

namespace zencxx { inline namespace thread { namespace details {
template class default_scheduler_impl<exclusive_lock, true>;
template class default_scheduler_impl<exclusive_lock, false>;
template class default_scheduler_impl<rw_lock, false>;
}                                                           // namespace details

template class default_scheduler<exclusive_lock>;
template class default_scheduler<rw_lock>;

}}                                                          // namespace thread, zencxx
