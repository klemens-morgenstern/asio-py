// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/execution_context.hpp>
#include <asio/execution_context.hpp>


namespace asio_py
{

void register_execution_context(pybind11::module_ &m)
{
    namespace py = pybind11;
    if (hasattr(m,"execution_context"))
        return;
    py::class_<asio::execution_context> ec(m, "execution_context");

    py::enum_<asio::execution_context::fork_event>(ec, "fork_event")
        .value("fork_prepare", asio::execution_context::fork_prepare, "Notify the context that the process is about to fork.")
        .value("fork_parent",asio::execution_context::fork_parent, "Notify the context that the process has forked and is the parent.")
        .value("fork_child",asio::execution_context::fork_child, "Notify the context that the process has forked and is the child.")
        ;

    ec
        .def(py::init<>())
        .def("notify_fork", &asio::execution_context::notify_fork)
        ;

    py::class_<asio::execution_context::service, std::unique_ptr<asio::execution_context::service, py::nodelete>>(ec, "service")
        .def("context", &asio::execution_context::service::context)
        ;

    py::register_exception<asio::service_already_exists>(m, "service_already_exists");
    py::register_exception<asio::invalid_service_owner>(m, "invalid_service_owner");
}


}