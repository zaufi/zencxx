/**
 * \file
 *
 * \brief Class \c zencxx::thread::details::thread_lock_tracker (extern templates)
 *
 * \date Fri Aug 23 07:54:55 MSK 2013 -- Initial design
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

#pragma once

// Project specific includes
#include <zencxx/thread/details/thread_lock_tracker.hh>

// Standard includes

namespace zencxx { inline namespace thread { namespace details {

extern template class thread_lock_tracker<exclusive_lock, 1ul>;
extern template class thread_lock_tracker<rw_lock, 2ul>;

}}}                                                         // namespace details, thread, zencxx

extern template class std::vector<boost::thread::id>;
extern template class std::array<std::vector<boost::thread::id>, 2ul>;
