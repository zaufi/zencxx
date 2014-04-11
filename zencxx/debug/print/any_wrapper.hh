/**
 * \file
 *
 * \brief Class \c zencxx::debug::print::any_wrapper (interface)
 *
 * \date Sun Jul 28 13:21:51 MSK 2013 -- Initial design
 */
/*
 * Copyright (C) 2010-2014 Alex Turbov and contributors, all rights reserved.
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

// Standard includes

namespace zencxx { namespace debug { namespace print {

/// A wrapper class to hold a value of a wrapped type \c T by value
template <typename T>
class any_wrapper
{
public:
    typedef T wrapped_type;
    any_wrapper(const wrapped_type value) : m_value(value) {}
    wrapped_type data() const
    {
        return m_value;
    }

private:
    const T m_value;
};

/// A wrapper class to hold a value of a wrapped type \c T by reference
template <typename T>
class any_wrapper<T&>
{
public:
    typedef T wrapped_type;
    any_wrapper(const wrapped_type& value) : m_value(value) {}
    const wrapped_type& data() const
    {
        return m_value;
    }

private:
    const wrapped_type& m_value;
};

/// A wrapper class to hold a value of a wrapped type \c T by pointer
template <typename T>
class any_wrapper<T*>
{
public:
    typedef T* wrapped_type;
    any_wrapper(const wrapped_type value) : m_value(value) {}
    wrapped_type data() const
    {
        return m_value;
    }

private:
    const wrapped_type m_value;
};

}}}                                                         // namespace print, debug, zencxx
