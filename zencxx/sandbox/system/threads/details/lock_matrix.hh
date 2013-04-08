/**
 * \file
 *
 * \brief Class lock_matrix (interface)
 *
 * \date Thu Jun 10 11:21:01 MSD 2010 -- Initial design
 */

#if !BOOST_PP_IS_ITERATING
#  ifndef __SYSTEM__THREADS__DETAILS__LOCK_MATRIX_HH__
#    define __SYSTEM__THREADS__DETAILS__LOCK_MATRIX_HH__

// Project specific includes
#    include <config.h>
#    include <system/threads/details/unilock_config.hh>

// Standard includes
#    include <boost/assert.hpp>
#    include <boost/mpl/at.hpp>
#    include <boost/mpl/int.hpp>
#    include <boost/mpl/size.hpp>
#    include <boost/preprocessor/cat.hpp>
#    include <boost/preprocessor/comma_if.hpp>
#    include <boost/preprocessor/iterate.hpp>
#    include <boost/preprocessor/repeat.hpp>
#    include <algorithm>

namespace sys { namespace threads { namespace details {

template <
    typename Sequence
  , int row
  , int col
  >
struct at_ij
{
    typedef typename boost::mpl::at<
        typename boost::mpl::at<
            Sequence
          , boost::mpl::int_<row>
          >::type::second
      , boost::mpl::int_<col>
      >::type type;
};

template <
    typename LockType
  , typename Sequence
  , int Size = boost::mpl::size<Sequence>::type::value
  >
class lock_matrix;

}}}                                                         // namespace details, threads, sys

#    define BOOST_PP_ITERATION_PARAMS_1 (3, (1, UNILOCK_MAX_LOCK_MATRIX_SIZE, "system/threads/details/lock_matrix.hh"))
#    include BOOST_PP_ITERATE()
#  endif                                                    // __SYSTEM__THREADS__DETAILS__LOCK_MATRIX_HH__
#else

#  define N BOOST_PP_ITERATION()

#  define LOCK_MATRIX_COL(z, n, text) \
    BOOST_PP_COMMA_IF(n) at_ij<Sequence, text, n>::type::value

#  define LOCK_MATRIX_ROW(z, n, text) \
    BOOST_PP_COMMA_IF(n) {BOOST_PP_REPEAT(N, LOCK_MATRIX_COL, n)}

#  define MUL_ROW(z, n, text) \
    BOOST_PP_IF(n, &&, ) (!(m_lock_matrix[t][n] || bool(m_state[n])) || m_lock_matrix[t][n])

#  define CHECK_ROW(z, n, text) \
    BOOST_PP_IF(n, ||, ) (text[n] > 0)

namespace sys { namespace threads { namespace details {

/// \todo Check that LockType is enum and Sequence is conforming to MPL Sequence concept
template <
    typename LockType
  , typename Sequence
  >
class lock_matrix<LockType, Sequence, N>
{
public:
    typedef LockType lock_type;
    typedef Sequence matrix_spec_type;
    typedef boost::mpl::int_<N> matrix_size;

    lock_matrix()
    {
        std::fill_n(m_state, N, 0);
    }
    void lock(lock_type t)
    {
        ++m_state[t];
    }
    void unlock(lock_type t)
    {
        --m_state[t];
        /// \todo Need to throw smth here! (resourse error?)
        BOOST_ASSERT("More unlock()s than lock()s??" && 0 <= m_state[t]);
    }
    bool can_lock(lock_type t) const
    {
        return BOOST_PP_REPEAT(N, MUL_ROW, _);
    }
    bool is_locked() const
    {
        return BOOST_PP_REPEAT(N, CHECK_ROW, m_state);
    }
    bool is_locked(lock_type t) const
    {
        return m_state[t] > 0;
    }

private:
    static const bool m_lock_matrix[N][N];
    int m_state[N];
};

template <
    typename LockType
  , typename Sequence
  >
const bool lock_matrix<LockType, Sequence, N>::m_lock_matrix[N][N] =
{
    BOOST_PP_REPEAT(N, LOCK_MATRIX_ROW, _)
};

}}}                                                         // namespace details, threads, sys
#  undef CHECK_ROW
#  undef MUL_ROW
#  undef LOCK_MATRIX_ROW
#  undef LOCK_MATRIX_COL
#  undef N
#endif                                                      // BOOST_PP_IS_ITERATING
