// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/cancellation_signal.hpp>
#include <asio/cancellation_signal.hpp>
#include <pybind11/operators.h>

namespace asio_py
{

void register_cancellation_signal(pybind11::module & m)
{
    namespace py = pybind11;
    py::class_<asio::cancellation_slot>(m, "cancellation_slot")
            .def(py::init<>())
            .def("assign", &asio::cancellation_slot::assign<py::function>)
            .def("clear", &asio::cancellation_slot::clear)
            .def("is_connected", &asio::cancellation_slot::is_connected)
            .def("has_handler", &asio::cancellation_slot::has_handler)
            .def(py::self == py::self)
            .def(py::self != py::self)
            ;


    py::class_<asio::cancellation_signal>(m, "cancellation_signal")
            .def(py::init<>())
            .def("emit", &asio::cancellation_signal::emit)
            .def("slot", &asio::cancellation_signal::slot)
            ;
}

}