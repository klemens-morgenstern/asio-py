// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/cancellation_type.hpp>
#include <asio/cancellation_type.hpp>
#include <pybind11/operators.h>

namespace asio_py
{

void register_cancellation_type(pybind11::module & m)
{
    namespace py = pybind11;
    py::enum_<asio::cancellation_type>(m, "cancellation_type")
            .value("none", asio::cancellation_type::none)
            .value("terminal", asio::cancellation_type::terminal)
            .value("partial", asio::cancellation_type::partial)
            .value("total", asio::cancellation_type::total)
            .value("all", asio::cancellation_type::all)
            .def(! py::self )
            .def(py::self & py::self )
            .def(py::self | py::self )
            .def(py::self ^ py::self )
            .def(~ py::self )
            .def(py::self &= py::self )
            .def(py::self |= py::self )
            .def(py::self ^= py::self )
            ;
}

}