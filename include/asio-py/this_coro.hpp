// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_THIS_CORO_HPP
#define ASIO_PY_THIS_CORO_HPP

#include <asio/this_coro.hpp>

namespace asio_py
{
void register_this_coro(pybind11::module_ & m);

}

#endif //ASIO_PY_THIS_CORO_HPP
