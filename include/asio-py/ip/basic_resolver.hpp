// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_BASIC_RESOLVER_HPP
#define ASIO_PY_BASIC_RESOLVER_HPP

#include <asio/ip/basic_resolver.hpp>
#include <asio-py/detail/typename.hpp>
#include <pybind11.h>

namespace asio_py
{

template<typename Protocol, typename Executor = asio::any_io_executor>
void register_basic_resolver(
        pybind11::module_ & m,
        const std::string & name = detail::get_sanitized_type_name<asio::ip::basic_resolver<Protocol>>())
{
    using resolver = asio::ip::basic_resolver<Protocol, Executor>;
    namespace py = pybind11;
    py::class_<resolver> cl(m, name.c_str());
}

}

#endif //ASIO_PY_BASIC_RESOLVER_HPP
