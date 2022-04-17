// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/detail/typename.hpp>
#include <asio-py/io_context.hpp>
#include <asio-py/helpers/options.hpp>

#include <asio/io_context.hpp>
#include <asio/io_context_strand.hpp>

#include <pybind11/chrono.h>
#include <pybind11/operators.h>


namespace asio_py
{

template<typename T, typename U> auto foo(T, U);

void register_io_context(pybind11::module_ & m)
{
    namespace py = pybind11;

    py::class_<asio::io_context, asio::execution_context> ioc(m, "io_context");
    py::class_<asio::io_context::executor_type > ioc_exec(ioc, "executor_type");


    using ms_t = std::chrono::milliseconds;
    using t1 = std::chrono::steady_clock::time_point;
    using t2 = std::chrono::system_clock::time_point;
    using t3 = std::chrono::high_resolution_clock::time_point;
    
    ioc .def(py::init<>())
        .def(py::init<int>(), py::arg("concurrency_hint"))
        .def("get_executor", &asio::io_context::get_executor)
        .def("run", py::overload_cast<>(&asio::io_context::run))
        .def("run", py::overload_cast<asio::error_code&>(&asio::io_context::run))
        .def("run_one", py::overload_cast<>(&asio::io_context::run_one))
        .def("run_one", py::overload_cast<asio::error_code&>(&asio::io_context::run_one))
        .def("run_for", &asio::io_context::run_for<ms_t::rep, ms_t::period>)
        .def("run_until", &asio::io_context::run_until<t1::clock, t1::duration>)
        .def("run_until", &asio::io_context::run_until<t2::clock, t2::duration>)
        .def("run_until", &asio::io_context::run_until<t3::clock, t3::duration>)
        .def("run_one_for", &asio::io_context::run_one_for<ms_t::rep, ms_t::period>)
        .def("run_one_until", &asio::io_context::run_one_until<t1::clock, t1::duration>)
        .def("run_one_until", &asio::io_context::run_one_until<t2::clock, t2::duration>)
        .def("run_one_until", &asio::io_context::run_one_until<t3::clock, t3::duration>)
        .def("poll", py::overload_cast<>(&asio::io_context::poll))
        .def("poll", py::overload_cast<asio::error_code&>(&asio::io_context::poll))
        .def("poll_one", py::overload_cast<>(&asio::io_context::poll_one))
        .def("poll_one", py::overload_cast<asio::error_code&>(&asio::io_context::poll_one))
        .def("stop", &asio::io_context::stop)
        .def("stopped", &asio::io_context::stopped)
        .def("restart", &asio::io_context::restart)
        .def("reset", &asio::io_context::reset)
        ;


    namespace ex = asio::execution;
    using executor_type = asio::io_context::executor_type;
    ioc_exec
        .def("running_in_this_thread",  &executor_type::running_in_this_thread)
        .def("on_work_started",  &executor_type::on_work_started)
        .def("on_work_finished",  &executor_type::on_work_finished)
        .def(py::self != py::self)
        .def(py::self == py::self)
        ;

    const auto mod =
            [&]<typename Modified>(register_tag<Modified >)
            {
                if constexpr (!std::is_same_v<typename asio::io_context::executor_type, Modified>)
                {
                    const auto nm = "__" + detail::get_sanitized_type_name<Modified>();
                    py::class_<Modified>(ioc, nm.c_str())
                        .def("running_in_this_thread",  &Modified::running_in_this_thread)
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

    using strand = asio::io_context::strand;

    py::class_<strand>(ioc, "strand")
            .def(py::init<asio::io_context &>())
            .def("context", &strand::context)
            .def("running_in_this_thread",  &strand::running_in_this_thread)
            .def("on_work_started",  &strand::on_work_started)
            .def("on_work_finished",  &strand::on_work_finished)
            ;
}

}