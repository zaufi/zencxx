/**
 * \file
 *
 * \brief Class fifo_scheduler (interface)
 *
 * \date Thu Jun 10 14:05:44 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__THREADS__FIFO_SCHEDULER_HH__
#  define __SYSTEM__THREADS__FIFO_SCHEDULER_HH__

// Project specific includes
#  include <config.h>
#  include <system/threads/details/lock_matrix.hh>

// Standard includes
#  include <boost/call_traits.hpp>
#  include <boost/fusion/include/at.hpp>
#  include <boost/fusion/include/vector.hpp>
#  include <algorithm>
#  include <list>

namespace sys { namespace threads {

template <typename MatrixSpec, typename Sequence = std::list<int> >
class fifo_scheduler
{
    typedef Sequence sequence_type;

public:
    typedef details::lock_matrix<
        typename MatrixSpec::type
      , typename MatrixSpec::matrix_type
      > matrix_type;

    typedef boost::fusion::vector<typename matrix_type::lock_type> lock_params_type;
    typedef boost::fusion::vector<typename matrix_type::lock_type> unlock_params_type;
    typedef typename sequence_type::value_type request_id_type;

    fifo_scheduler() : m_next_id(request_id_type()) {}

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
        m_queue.insert(m_queue.end(), rid = m_next_id++);
        return rid;
    }
    void unassign_request_id(typename boost::call_traits<lock_params_type>::param_type, request_id_type request_id)
    {
        typename sequence_type::iterator it;
        if ((it = std::find(m_queue.begin(), m_queue.end(), request_id)) != m_queue.end())
            m_queue.erase(it);
    }

private:
    matrix_type m_matrix;
    sequence_type m_queue;
    request_id_type m_next_id;
};

}}                                                          // namespace threads, sys
#endif                                                      // __SYSTEM__THREADS__FIFO_SCHEDULER_HH__
