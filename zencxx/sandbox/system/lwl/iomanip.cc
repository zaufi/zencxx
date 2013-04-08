/**
 * \file
 *
 * \brief Class iomanip (implementation)
 *
 * \date Sat Jul 17 10:52:48 MSD 2010 -- Initial design
 */

// Project specific includes
#include <config.h>
#include <system/lwl/iomanip.hh>
#include <system/lwl/details/indentation_data.hh>
#include <system/threads/specific.hh>

// Standard includes
#  include <cassert>

namespace sys { namespace lwl { namespace {

typedef sys::threads::specific_val<details::indentation_data> indentaion_storage_type;
indentaion_storage_type s_logger_indent_info;

/**
 * \brief Get pointer to TSL value with indentation info
 *
 * If no \c indentation_data was attached to stream yet, do it!
 * So u mustn't to attach it explicitly to every stream where u wanna use
 * \c indent or \c unindent manipulators.
 */
details::indentation_data& get_indentation_data(std::ostream& os)
{
    // Try to access stream's attached data that holds indentation info
    indentaion_storage_type* ptr = static_cast<indentaion_storage_type*>(
        os.pword(details::indentation_data::get_index())
      );
    if (!ptr)                                               // Is smth atached?
    {
        os.pword(details::indentation_data::get_index()) = &s_logger_indent_info;
        // Try again...
        ptr = static_cast<indentaion_storage_type*>(os.pword(details::indentation_data::get_index()));
    }
    return ptr->value();
}

}                                                           // anonymous namespace

std::ostream& indent(std::ostream& os)
{
    details::indentation_data& ii = get_indentation_data(os);
    ++ii.m_current_indent_level;
    return os;
}

std::ostream& unindent(std::ostream& os)
{
    details::indentation_data& ii = get_indentation_data(os);
    assert("Sanity check!" && ii.m_current_indent_level != 0);
    if (ii.m_current_indent_level)                          // Do not allow to counter to cross zero!
        --ii.m_current_indent_level;
    return os;
}

std::ostream& endl(std::ostream& os)
{
    os << '\n';                                             // Move to new line as generic std::endl do

    details::indentation_data& ii = get_indentation_data(os);
    const std::string::size_type indent = ii.m_current_indent_level * ii.m_indent_size;
    if (indent)
    {
        std::string indentation(indent, ' ');
        os << indentation;
    }
    return os;
}

}}                                                          // namespace lwl, sys
