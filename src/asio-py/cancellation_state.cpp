// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/cancellation_state.hpp>
#include <asio/cancellation_state.hpp>
#include <pybind11/operators.h>

namespace asio_py
{

void register_cancellation_state(pybind11::module & m)
{
    namespace py = pybind11;
    m.def("cancellation_filter", [](unsigned int mask) -> pybind11::function
                                 {
                                    return py::cpp_function([mask = static_cast<asio::cancellation_type_t>(mask)](
                                            asio::cancellation_type_t type)
                                        {
                                        return type & mask;
                                        });
                                 });

    py::class_<asio::disable_cancellation>        (m, "disable_cancellation")
            .def("__call", &asio::disable_cancellation::operator());
    py::class_<asio::enable_terminal_cancellation>(m, "enable_terminal_cancellation")
            .def("__call", &asio::enable_terminal_cancellation::operator());
    py::class_<asio::enable_partial_cancellation> (m, "enable_partial_cancellation")
            .def("__call", &asio::enable_partial_cancellation::operator());
    py::class_<asio::enable_total_cancellation>   (m, "enable_total_cancellation")
            .def("__call", &asio::enable_total_cancellation::operator());

    using f = std::function<asio::cancellation_type(asio::cancellation_type)>;

    py::class_<asio::cancellation_state>(m, "cancellation_state")
            .def(py::init<>())
            .def(py::init<asio::cancellation_slot>(), py::arg("slot"))
            .def(py::init<asio::cancellation_slot, f>(), py::arg("slot"), py::arg("filter"))
            .def(py::init<asio::cancellation_slot, f, f>(), py::arg("slot"), py::arg("in_filter"), py::arg("out_filter"))
            .def("slot", &asio::cancellation_state::slot)
            .def("is_connected", &asio::cancellation_state::cancelled)
            .def("clear", &asio::cancellation_state::clear, py::arg("mask") = asio::cancellation_type::all)
            .def("cancelled", &asio::cancellation_state::cancelled)
            ;
}

}