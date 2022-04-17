// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio/detached.hpp>
#include <pybind11.h>

namespace asio_py
{

void register_detached(pybind11::module & m)
{
    namespace py = pybind11;
    py::class_<asio::detached_t>(m, "detached_t");
    m.attr("detached") = asio::detached;

}

}