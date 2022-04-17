// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_CANCELLATION_TYPE_HPP
#define ASIO_PY_CANCELLATION_TYPE_HPP

#include <pybind11.h>

namespace asio_py
{

void register_cancellation_type(pybind11::module & m);

}

#endif //ASIO_PY_CANCELLATION_TYPE_HPP
