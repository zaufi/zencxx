/**
 * \file
 *
 * \brief Class \c zencxx::default_scheduler (interface)
 *
 * \date Wed Jul 17 11:53:02 MSK 2013 -- Initial design
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

#ifndef __ZENCXX__THREAD__DEFAULT_SCHEDULER_HH__
# define __ZENCXX__THREAD__DEFAULT_SCHEDULER_HH__

// Project specific includes
# include <zencxx/thread/details/lock_matrix.hh>

// Standard includes

namespace zencxx {

/**
 * \brief [Type brief class description here]
 *
 * [More detailed description here]
 *
 */
template <typename MatrixSpec>
class default_scheduler
{
public:
    typedef MatrixSpec matrix_type;
    typedef typename matrix_type::type lock_param_type;
    typedef typename matrix_type::type unlock_param_type;

    bool try_lock(const int, const lock_param_type p)
    {
        const auto result = m_matrix.can_lock(p);
        if (result)
            m_matrix.lock(p);
        return result;
    }
    void unlock(const unlock_param_type p)
    {
        m_matrix.unlock(p);
    }
    //
    bool is_used() const
    {
        return m_matrix.is_locked();
    }
    // unused by this policy
    int assign_request_id(const lock_param_type)
    {
        return 0;
    }
    // unused by this policy
    void unassign_request_id(const int, const lock_param_type)
    {}

private:
    thread::details::lock_matrix<matrix_type> m_matrix;
};

}                                                           // namespace zencxx
#endif                                                      // __ZENCXX__THREAD__DEFAULT_SCHEDULER_HH__
