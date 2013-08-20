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
# include <zencxx/thread/details/has_default_lock_param.hh>
# include <zencxx/thread/details/lock_matrix.hh>

// Standard includes

namespace zencxx { inline namespace thread { namespace details {
/**
 * \brief Default scheduler implementation
 *
 * This is generic template which assume that given matrix type \c MatrixSpec
 * has no default lock type.
 *
 */
template <typename MatrixSpec, bool HasDefaultLockParam>
class default_scheduler_impl
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

/**
 * \brief Specialization of \c default_scheduler_impl for
 * \c MatrixSpec types w/ default lock type specified.
 *
 * One (and I think the only) such \c MatrixSpec is an \c zencxx::thread::exclusive_lock.
 * Having the only lock type makes some redundancy when it (type) used by a scheduler.
 * The purpose of this specialization is to remove necessity to specify a lock type
 * in case when it is the only that matrix have.
 */
template <typename MatrixSpec>
class default_scheduler_impl<MatrixSpec, true>
  : public default_scheduler_impl<MatrixSpec, false>
{
    typedef default_scheduler_impl<MatrixSpec, false> base_class;
public:
    typedef typename base_class::matrix_type matrix_type;

    // Bring inherited members into the scope
    using base_class::try_lock;
    using base_class::unlock;
    using base_class::is_used;
    using base_class::assign_request_id;
    using base_class::unassign_request_id;

    /// \name Overloads w/ default lock type
    //@{
    bool try_lock(const int dummy)
    {
        return this->try_lock(dummy, matrix_type::default_lock);
    }
    void unlock()
    {
        this->unlock(matrix_type::default_lock);
    }
    // unused by this policy
    int assign_request_id()
    {
        return this->assign_request_id(matrix_type::default_lock);
    }
    // unused by this policy
    void unassign_request_id(const int dummy)
    {
        this->unassign_request_id(dummy, matrix_type::default_lock);
    }
    //@}
};

}                                                           // namespace details

/**
 * \brief Default locking policy (scheduler)
 *
 * This class implements <em>no special behavior</em> locking policy.
 * This mean lock/unlock requests are handled as it they were addressed
 * to ordinal mutex.
 *
 */
template <typename MatrixSpec>
class default_scheduler
  : public details::default_scheduler_impl<
      MatrixSpec
    , details::has_default_lock_param<MatrixSpec>::value
    >
{
};

}}                                                          // namespace thread, zencxx
#endif                                                      // __ZENCXX__THREAD__DEFAULT_SCHEDULER_HH__
