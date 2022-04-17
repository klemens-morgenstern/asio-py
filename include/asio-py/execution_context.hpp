// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_EXECUTION_CONTEXT_HPP
#define ASIO_PY_EXECUTION_CONTEXT_HPP

#include <pybind11.h>
#include <asio/execution_context.hpp>

namespace asio_py
{

template<typename T>
inline void register_service(pybind11::module_ & m)
{
    m.def("use_service", pybind11::overload_cast<asio::execution_context>(&asio::use_service<T>));
    m.def("use_service", pybind11::overload_cast<asio::io_context>(&asio::use_service<T>));
    m.def("add_service", &asio::use_service<T>);
    m.def("has_service", &asio::use_service<T>);
}

void register_execution_context(pybind11::module_ & m);


}

#endif //ASIO_PY_EXECUTION_CONTEXT_HPP
