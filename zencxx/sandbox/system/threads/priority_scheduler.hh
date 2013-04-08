/**
 * \file
 *
 * \brief Class priority_scheduler (interface)
 *
 * \date Thu Jun 10 14:06:03 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__PRIORITY_SCHEDULER_HH__
#  define __SYSTEM__THREADS__PRIORITY_SCHEDULER_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/lock_matrix.hh>

// Standard includes
#  include <boost/call_traits.hpp>
#  include <boost/fusion/include/at.hpp>
#  include <boost/fusion/include/vector.hpp>
#  include <algorithm>
#  include <list>
#  include <queue>

namespace sys { namespace threads {

template <
    typename MatrixSpec
  , typename Sequence = std::list<int>
  , typename Compare = std::less<typename Sequence::value_type>
  >
class priority_scheduler
{
    typedef std::priority_queue<
        typename Sequence::value_type
      , Sequence
      , Compare
      > sequence_type;

public:
    typedef typename sequence_type::value_type request_id_type;

    typedef details::lock_matrix<
        typename MatrixSpec::type
      , typename MatrixSpec::matrix_type
      > matrix_type;

    typedef boost::fusion::vector<
        request_id_type
      , typename matrix_type::lock_type
      > lock_params_type;
    typedef boost::fusion::vector<typename matrix_type::lock_type> unlock_params_type;

    priority_scheduler() : m_next_id(request_id_type()) {}

    bool try_lock(typename boost::call_traits<lock_params_type>::param_type p, request_id_type request_id)
    {
        bool result = (*m_queue.begin() == request_id && m_matrix.can_lock(boost::fusion::at_c<0>(p)));
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
        /// \todo Need to add assert here...
        return !m_queue.empty() || m_matrix.is_locked();
    }
    int assign_request_id(typename boost::call_traits<lock_params_type>::param_type)
    {
        int rid;
        m_queue.push(rid = m_next_id++);
        return rid;
    }
    void unassign_request_id(typename boost::call_traits<lock_params_type>::param_type, request_id_type request_id)
    {
        /// \todo Huh... I don't remember what's going on here... Does this check will be succeed sometime?
        if (m_queue.top() == request_id) m_queue.pop();
    }

private:
    matrix_type m_matrix;
    sequence_type m_queue;
    request_id_type m_next_id;
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__PRIORITY_SCHEDULER_HH__
