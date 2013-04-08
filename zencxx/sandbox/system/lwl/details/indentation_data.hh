/**
 * \file
 *
 * \brief Type to hold stream's attached indentation related data
 *
 * \date Sat Jul 17 10:54:42 MSD 2010 -- Initial design
 */

#ifndef __SYSTEM__LWL__DETAILS__INDENTATION_DATA_HH__
#  define __SYSTEM__LWL__DETAILS__INDENTATION_DATA_HH__

// Project specific includes
#  include <config.h>

// Standard includes
#  include <ios>

namespace sys { namespace lwl { namespace details {

class log_proxy;

/**
 * \brief Type to hold stream's attached indentation related data
 *
 * An instances of this class would be attached to any steam used for logging
 * purposes. It just holds current indentation level and size.
 *
 * \internal
 */
class indentation_data
{
public:
    static const int DEFAULT_INDENT_SIZE = 4;               ///< Default value for indentaion size

    unsigned m_current_indent_level;
    unsigned m_indent_size;

    /// This constructor helps to have a default value for TLS storage
    /// on first touch...
    explicit indentation_data(unsigned l = 0, unsigned s = DEFAULT_INDENT_SIZE)
      : m_current_indent_level(l), m_indent_size(s)
    {}

    /**
     * \brief Return index into stream's state array that belongs to indentation data.
     *
     * This function will allocate it on first call.
     */
    static int get_index()
    {
        static const int S_IDX = std::ios_base::xalloc();
        return S_IDX;
    }
};

}}}                                                         // namespace details, lwl, sys
#endif                                                      // __SYSTEM__LWL__DETAILS__INDENTATION_DATA_HH__
