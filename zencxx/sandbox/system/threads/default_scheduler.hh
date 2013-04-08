/**
 * \file
 *
 * \brief Class default_scheduler (interface)
 *
 * \date Thu Jun 10 12:36:33 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__DEFAULT_SCHEDULER_HH__
#  define __SYSTEM__THREADS__DEFAULT_SCHEDULER_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/lock_matrix.hh>

// Standard includes
#  include <boost/call_traits.hpp>
#  include <boost/fusion/include/at.hpp>
#  include <boost/fusion/include/vector.hpp>

namespace sys { namespace threads {

template <typename MatrixSpec>
class default_scheduler
{
public:
    typedef details::lock_matrix<
        typename MatrixSpec::type
      , typename MatrixSpec::matrix_type
      > matrix_type;

    typedef boost::fusion::vector<typename matrix_type::lock_type> lock_params_type;
    typedef boost::fusion::vector<typename matrix_type::lock_type> unlock_params_type;
    typedef int request_id_type;

    bool try_lock(typename boost::call_traits<lock_params_type>::param_type p, request_id_type)
    {
        bool result = m_matrix.can_lock(boost::fusion::at_c<0>(p));
        if (result) m_matrix.lock(boost::fusion::at_c<0>(p));
        return result;
    }
    void unlock(typename boost::call_traits<unlock_params_type>::param_type p)
    {
        m_matrix.unlock(boost::fusion::at_c<0>(p));
    }
    //
    bool is_used() const
    {
        return m_matrix.is_locked();
    }
    // unused by this policy
    request_id_type assign_request_id(typename boost::call_traits<lock_params_type>::param_type)
    {
        return request_id_type();
    }
    // unused by this policy
    void unassign_request_id(typename boost::call_traits<lock_params_type>::param_type, request_id_type)
    {}

private:
    matrix_type m_matrix;
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__DEFAULT_SCHEDULER_HH__
