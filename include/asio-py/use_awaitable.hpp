// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_USE_AWAITABLE_HPP
#define ASIO_PY_USE_AWAITABLE_HPP

#include <asio/use_awaitable.hpp>
#include <asio-py/detail/typename.hpp>

#include <pybind11.h>

namespace asio_py
{

template<typename Executor = asio::any_io_executor>
inline void register_use_awaitable(pybind11::module & m,
                                   std::string name = "use_awaitable_" + detail::get_sanitized_type_name<Executor>())
{
    const auto name_t = name + "_t";
    pybind11::class_<asio::use_awaitable_t<Executor>>(m, name_t.c_str());
    m.attr(name.c_str()) = asio::use_awaitable_t<Executor>();

}

template<>
inline void register_use_awaitable<asio::any_io_executor>(pybind11::module & m, std::string)
{
    pybind11::class_<asio::use_awaitable_t<>>(m, "use_awaitable_t");
    m.attr("use_awaitable") = asio::use_awaitable;
}

}

#endif //ASIO_PY_USE_AWAITABLE_HPP
