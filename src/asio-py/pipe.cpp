// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/pipe.hpp>

namespace asio_py
{

void register_readable_pipe(pybind11::module_ & m)
{
    register_basic_readable_pipe(m, "readable_pipe");
}

void register_writable_pipe(pybind11::module_ & m)
{
    register_basic_writable_pipe(m, "writable_pipe");
}

}