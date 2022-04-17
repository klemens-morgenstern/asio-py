// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_DISPATCH_HPP
#define ASIO_PY_DISPATCH_HPP

#include <pybind11.h>
#include <asio/any_io_executor.hpp>
#include <asio/dispatch.hpp>
#include "asio-py/helpers/tag.hpp"
#include "asio-py/completion_token.hpp"

namespace asio_py
{


inline void register_dispatch(pybind11::module_ & m)
{
    register_async_op(m, "dispatch",
                      register_tag<asio::any_io_executor>{},
                      [](asio::any_io_executor exec, auto tk)
                      {
                          return asio::dispatch(exec, std::move(tk));
                      },
                      pybind11::arg("executor"));

    register_async_op(m, "dispatch",
                      register_tag<>{},
                      [](auto tk)
                      {
                          return asio::dispatch(std::move(tk));
                      });
}


}

#endif //ASIO_PY_DISPATCH_HPP
