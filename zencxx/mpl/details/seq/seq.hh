/**
 * \file
 *
 * \brief Variadic sequence specializations
 *
 * \date Mon Jul  2 14:00:30 MSK 2012 -- Initial design
 */
/*
 * Copyright (C) 2012-2014 Alex Turbov and contributors, all rights reserved.
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
#include <zencxx/mpl/seq_fwd.hh>
#include <zencxx/mpl/details/seq/at.hh>
#include <zencxx/mpl/details/seq/back_front.hh>
#include <zencxx/mpl/details/seq/adv_dist.hh>
#include <zencxx/mpl/details/seq/begin_end.hh>
#include <zencxx/mpl/details/seq/clear.hh>
#include <zencxx/mpl/details/seq/copy_range_impl.hh>
#include <zencxx/mpl/details/seq/empty.hh>
#include <zencxx/mpl/details/seq/O1_size.hh>
#include <zencxx/mpl/details/seq/push_pop.hh>
#include <zencxx/mpl/details/seq/size.hh>

/// \note No need to specialize \c next or \c prior, cuz default implementation
/// will use iterator's \c next/prior types which are defined for \c seq_iter as well.

/// \note No need to specialize \c deref, default implementation just takes nested \c type of the
/// iterator type, which is pretty fine for \c seq_iter type.

// Standard includes
