// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_USE_FUTURE_HPP
#define ASIO_PY_USE_FUTURE_HPP

#include <asio/use_future.hpp>
#include <asio-py/detail/typename.hpp>

#include <pybind11.h>

namespace asio_py
{

inline void register_use_future(pybind11::module & m)
{
    pybind11::class_<asio::use_future_t<>>(m, "use_future_t");
    m.attr("use_future") = asio::use_future;
}

}


#endif //ASIO_PY_USE_FUTURE_HPP
