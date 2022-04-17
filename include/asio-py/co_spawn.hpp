// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_CO_SPAWN_HPP
#define ASIO_PY_CO_SPAWN_HPP

#include <pybind11.h>

#include <asio/co_spawn.hpp>
#include <asio-py/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include "completion_token.hpp"

namespace asio_py
{

asio::awaitable<pybind11::object> adopt_async_func(pybind11::object obj);

inline void register_co_spawn(pybind11::module_ & m)
{
    namespace py = pybind11;
    /*
     * This can be done without C++20 as well, by introducing a custom python completer. For now we'll default to
     * use_awaitable though, because it's easier & probably more efficient.
     */
    // can't be done with completion_token

    register_async_op(m, "co_spawn", register_tag<asio::any_io_executor, awaitable_wrapper&>{},
                      [](asio::any_io_executor exec, awaitable_wrapper& aw, auto tk)
                      {
                            return asio::co_spawn(exec, std::move(aw).await(), std::move(tk));
                      },
                      py::arg("executor"),
                      py::arg("awaitable"));

    register_async_op(m, "co_spawn",
          register_tag<asio::any_io_executor, pybind11::object>{},
          [](asio::any_io_executor exec, pybind11::object obj, auto token)
          {
              return asio::co_spawn(exec, adopt_async_func(obj), std::move(token));
          },
          py::arg("executor"),
          py::arg("awaitable"));

}

}

#endif //ASIO_PY_CO_SPAWN_HPP
