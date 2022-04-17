// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pybind11.h>
#include <asio-py/this_coro.hpp>
#include "asio-py/execution.hpp"
#include "asio-py/register_all.hpp"

PYBIND11_MODULE(asio, m)
{
    asio_py::register_all(m);
}
