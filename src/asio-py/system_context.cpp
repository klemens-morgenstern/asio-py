// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/helpers/options.hpp>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/detail/typename.hpp>
#include <asio-py/system_context.hpp>

#include <asio/system_context.hpp>

#include <pybind11/operators.h>

namespace asio_py
{

void register_system_context(pybind11::module_ & m)
{
    namespace py = pybind11;

    py::class_<asio::system_context, asio::execution_context> ioc(m, "system_context");
    py::class_<asio::system_context::executor_type > ioc_exec(ioc, "executor_type");

    ioc .def(py::init<>())
            .def("get_executor", &asio::system_context::get_executor)
            .def("join", &asio::system_context::join)
            .def("stop", &asio::system_context::stop)
            .def("stopped", &asio::system_context::stopped)
            ;


    namespace ex = asio::execution;
    using executor_type = asio::system_context::executor_type;
    ioc_exec
            .def(py::init<>())
            .def("context",  &executor_type::context)
            .def("on_work_started",  &executor_type::on_work_started)
            .def("on_work_finished",  &executor_type::on_work_finished)
            .def(py::self != py::self)
            .def(py::self == py::self)
            ;


    const auto mod =
            [&]<typename Modified>(register_tag<Modified >)
            {
                if constexpr (!std::is_same_v<executor_type, Modified>)
                {
                    const auto nm = "__" + detail::get_sanitized_type_name<Modified>();
                    py::class_<Modified>(ioc, nm.c_str())
                            .def("context",                 &Modified::context)
                            .def("on_work_started",         &Modified::on_work_started)
                            .def("on_work_finished",        &Modified::on_work_finished)
                            .def(py::self != py::self)
                            .def(py::self == py::self)
                            ;
                }
            };

    [&]<typename ... Modifiers>(register_tag<Modifiers...>)
    {
        (mod(register_tag<Modifiers>{}), ...);
    }(detail::apply_properties_t<executor_type, executor_properties>{});
}

}