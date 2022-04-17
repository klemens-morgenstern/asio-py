// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/helpers/options.hpp>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/detail/typename.hpp>
#include <asio-py/thread_pool.hpp>

#include <asio/thread_pool.hpp>
#include <pybind11/operators.h>

namespace asio_py
{

void register_thread_pool(pybind11::module_ & m)
{
    namespace py = pybind11;

    py::class_<asio::thread_pool, asio::execution_context> ioc(m, "thread_pool");
    py::class_<asio::thread_pool::executor_type > ioc_exec(ioc, "executor_type");

    ioc .def(py::init<>())
            .def("attach", &asio::thread_pool::attach)
            .def("get_executor", &asio::thread_pool::get_executor)
            .def("executor", &asio::thread_pool::executor)
            .def("join", &asio::thread_pool::join)
            .def("scheduler", &asio::thread_pool::stop)
            .def("stop", &asio::thread_pool::stop)
            .def("wait", &asio::thread_pool::stop)
            ;

    namespace ex = asio::execution;
    using executor_type = asio::thread_pool::executor_type;
    ioc_exec.def("bulk_execute",  &executor_type::bulk_execute<py::function>)
            .def("context",  &executor_type::context)
            .def("on_work_started",  &executor_type::on_work_started)
            .def("on_work_finished",  &executor_type::on_work_finished)
            .def("running_in_this_thread",  &executor_type::running_in_this_thread)
            .def("schedule",  &executor_type::schedule)
            .def(py::self != py::self)
            .def(py::self == py::self)
            ;

    const auto mod =
            [&]<typename Modified>(register_tag<Modified >)
            {
                if constexpr (!std::is_same_v<typename asio::thread_pool::executor_type, Modified>)
                {
                    const auto nm = "__" + detail::get_sanitized_type_name<Modified>();
                    py::class_<Modified>(ioc, nm.c_str())
                        .def("bulk_execute",            &Modified::template bulk_execute<py::function>)
                        .def("context",                 &Modified::context)
                        .def("on_work_started",         &Modified::on_work_started)
                        .def("on_work_finished",        &Modified::on_work_finished)
                        .def("running_in_this_thread",  &Modified::running_in_this_thread)
                        .def("schedule",                &Modified::schedule)
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