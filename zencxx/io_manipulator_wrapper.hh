/**
 * \file
 *
 * \brief Helper macros to produce wrapper types for I/O stream manipulators
 *
 * \date Sun Mar 31 15:26:28 MSK 2013 -- Initial design
 *
 * \todo Add ability to hide a wrapped type into a namespace (like \c details or \c aux)
 */
/*
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

// Standard includes

/**
 * Function-like macro to declare a wrapper class which
 * holds a reference to \c Type.
 */
# define ZENCXX_MAKE_WRAPPER_CLASS(WrapperType, Type)     \
    class WrapperType                                     \
    {                                                     \
        const Type& m_ref;                                \
    public:                                               \
        typedef Type wrapped_type;                        \
        explicit WrapperType(const Type& r) : m_ref(r) {} \
        const Type& ref() const                           \
        {                                                 \
            return m_ref;                                 \
        }                                                 \
    }

/**
 * Function-like macro to declare a wrapper class for \c Type and a helper
 * function ("manipulator") to produce it from a "raw" type.
 *
 * Wrapper just holds a \c const reference to a wrapped type and has the
 * only member \c ref() to access it. Also manipulator-like function
 * will be defined to produce that wrappers from a "raw" type.
 *
 * It's supposed to be used like this:
 * \code
 *  struct some_user_type {...};
 *  ZENCXX_MAKE_IOMAIP_WRAPPER(manip_name, wrapped_type, some_user_type)
 *  std::ostream& operator<<(std::ostream& os, const wrapped_type& w)
 *  {
 *      // access/print members of original type (some_user_type) via w.ref()
 *      os << w.ref().m_member;
 *      return os;
 *  }
 * \endcode
 *
 * The basic idea behind all this stuff is like the following:
 * suppose you have a bunch of classes w/ UUID (for example) -- smth like a
 * \c document, \c session, \c user, ... The idea is to have a single, I/O
 * manipulator-like function to print an UUID for all of that types, possible
 * defined in different namespaces.
 *
 * To achieve that, one have to do the following:
 * \code
 *  namespace some {
 *  class document {};
 *  ZENCXX_MAKE_IOMAIP_WRAPPER(uuid, print_document_uuid, document)
 *  std::ostream& operator<<(std::ostream&, print_document_uuid&& w)
 *  {
 *     os << w.ref().uuid();
 *     return os;
 *  }
 *  }
 *  namespace other {
 *  class session {};
 *  ZENCXX_MAKE_IOMAIP_WRAPPER(uuid, print_session_uuid, session)
 *  std::ostream& operator<<(std::ostream&, print_session_uuid&& w)
 *  {
 *     os << w.ref().uuid();
 *     return os;
 *  }
 *  }
 *  // When somewhere else...
 *  some::document doc = ...;
 *  other::session s = ...;
 *  // Now you can print them in a similar way:
 *  std::cout << "session " << uuid(s) << ": got document " << uuid(doc) << std::endl;
 * \endcode
 *
 * Note what you don't need to specify a full namespace for manipulator function -- it
 * will be found by ADL. More complex example may print something else (except UUID) --
 * just suppose each of mentioned structures have a lot of fields and for logging purposes
 * from time to time you want to print some brief info about document/session/user
 * in form of <code>description[UUID]</code>, for example. To do so, you can have a
 * "manipulator" named \c brief() for all of that classes.
 *
 *
 * \param MkWrapperName an I/O manipulator-like function name
 * \param WrapperType a name of the wrapper type
 * \param Type a type to be wrapped
 *
 */
# define ZENCXX_MAKE_IOMAIP_WRAPPER(MkWrapperName, WrapperType, Type) \
    ZENCXX_MAKE_WRAPPER_CLASS(WrapperType, Type);                     \
    inline WrapperType MkWrapperName(const Type& r)                   \
    {                                                                 \
        return WrapperType(r);                                        \
    }

/**
 * Function-like macro to declare a template wrapper class which
 * holds a reference to \c Type<...> instantiated with parameters.
 * So, \c Type is a template tempalte parameter.
 */
# define ZENCXX_MAKE_TEMPLATE_TEMPLATE_WRAPPER_CLASS(WrapperType, Type) \
    template <typename... Params>                                       \
    class WrapperType                                                   \
    {                                                                   \
        const Type<Params...>& m_ref;                                   \
    public:                                                             \
        typedef Type<Params...> wrapped_type;                           \
        explicit WrapperType(const Type<Params...>& r) : m_ref(r) {}    \
        const Type<Params...>& ref() const                              \
        {                                                               \
            return m_ref;                                               \
        }                                                               \
    }

/**
 * Same as \c ZENCXX_MAKE_IOMAIP_WRAPPER, but produce a templated wrapper
 * and manipulator-like function w/ variadic parameters count.
 *
 * \param MkWrapperName name of the (template) function to make a wrapper.
 * It will be a "manupilator"'s name actually;
 * \param WrapperType name of the wrapper type. This is a template class
 * which just forward all parameters to a \c Type and holds a reference
 * given to a constructor as a data member;
 * \param Type a template name to wrap.
 *
 * \sa \c ZENCXX_MAKE_IOMAIP_WRAPPER
 */
# define ZENCXX_MAKE_IOMAIP_WRAPPER_TEMPLATE(MkWrapperName, WrapperType, Type) \
    ZENCXX_MAKE_TEMPLATE_TEMPLATE_WRAPPER_CLASS(WrapperType, Type);            \
    template <typename... Params>                                              \
    inline WrapperType<Params...> MkWrapperName(const Type<Params...>& r)      \
    {                                                                          \
        return WrapperType<Params...>(r);                                      \
    }

/**
 * Function-like macro to declare a template wrapper class for any
 * arbitrary type \c Type, which holds a reference to it.
 */
# define ZENCXX_MAKE_TEMPLATE_WRAPPER_CLASS(WrapperType, Type) \
    template <typename Type>                                   \
    class WrapperType                                          \
    {                                                          \
        const Type& m_ref;                                     \
    public:                                                    \
        typedef Type wrapped_type;                             \
        explicit WrapperType(const Type& r) : m_ref(r) {}      \
        const Type& ref() const                                \
        {                                                      \
            return m_ref;                                      \
        }                                                      \
    }
