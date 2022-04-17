// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_TYPENAME_HPP
#define ASIO_PY_TYPENAME_HPP

#include <string>
#include <string_view>

#include <pybind11.h>

namespace asio_py::detail
{

template < typename T >
constexpr inline std::string_view
get_type_name()
{
    // copied from sml
    constexpr std::string_view internal_name = __PRETTY_FUNCTION__;
#if defined(_MSC_VER) && !defined(__clang__)
    constexpr auto prefix = 26u, postfix = 7u;
#elif defined(__clang__)   // 63
    constexpr auto prefix = 55u, postfix = 1u;
#elif defined(__GNUC__)
    constexpr auto prefix = 55u, postfix = 1u;
#else
#error Unknown compiler
#endif

    return internal_name.substr(prefix, internal_name.size() - prefix - postfix);
};


template < typename T >
constexpr static std::string_view scoped_name = get_type_name< T >();

constexpr auto
split_name_and_scope_match(std::string_view name)
{
    std::size_t scope_end  = 0u;   // start
    std::size_t name_begin = 0u;

    constexpr auto is_alpha = [](char c)
    { return (('A' <= c) && ('Z' >= c)) || (('a' <= c) && ('z' >= c)) || (('0' <= c) && ('9' >= c)) || (c == '_'); };

    for (auto i = 0u; i < name.size(); i++)
    {
        if (is_alpha(name[i]))
            continue;
        else if (name.substr(i).starts_with("::"))
        {
            scope_end  = i;
            name_begin = i + 2;
            i += 1;
        }
        else
            break;
    }
    return std::pair(name.substr(0, scope_end), name.substr(name_begin, name.size() - name_begin));
}

template < typename T >
constexpr std::pair< std::string_view, std::string_view >
split_name_and_scope()
{
    return split_name_and_scope_match(scoped_name< T >);
}

template < typename T >
constexpr static std::string_view unscoped_name = std::get< 1 >(split_name_and_scope< T >());

template < typename T >
constexpr static std::string_view scope_of_type = std::get< 0 >(split_name_and_scope< T >());


template < typename T >
std::string
get_sanitized_type_name()
{
    constexpr auto nv = get_type_name< T >();
    std::string sv { nv };   // C++ name

    const char cs[] = ":<>(), ";
    for (auto & c : sv)
        if (auto itr = std::find(std::begin(cs), std::end(cs), c); itr != std::end(cs))
            c = '_';

    return sv;
}

template <>
inline std::string
get_sanitized_type_name< std::string >()
{
    return "str";
}

template < typename T >
inline pybind11::class_< std::decay_t< T > >
register_with_sanitized_type_name(pybind11::module_ &m)
{
    auto name = get_sanitized_type_name< std::decay_t< T > >();
    auto onm  = get_type_name< std::decay_t< T > >();
    return pybind11::class_< std::decay_t< T > >(m, name.c_str(), onm.data());
}

}

#endif //ASIO_PY_TYPENAME_HPP
