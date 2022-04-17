// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_TIMER_HPP
#define ASIO_PY_TIMER_HPP

#include <pybind11.h>
#include <pybind11/chrono.h>
#include <asio/basic_deadline_timer.hpp>
#include <asio/basic_waitable_timer.hpp>
#include <asio-py/completion_token.hpp>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/detail/typename.hpp>
#include <asio/thread_pool.hpp>


namespace asio_py
{

template<typename Clock, typename WaitTraits, typename Executor>
void register_basic_waitable_timer(pybind11::module_ & m,
                                   const std::string &name = detail::get_sanitized_type_name<asio::basic_waitable_timer<Clock, WaitTraits, Executor>>())
{
    using timer_t = asio::basic_waitable_timer<Clock, WaitTraits, Executor>;
    namespace py = pybind11;
    py::class_<timer_t> cl(m, name.c_str());

    cl.attr("duration")      = py::cast(typename timer_t::duration()).get_type();
    cl.attr("executor_type") = py::type::of<typename timer_t::executor_type>();
    cl.attr("time_point")    = py::cast(typename timer_t::time_point()).get_type();

    register_async_op(m, "async_wait",
                     register_tag<timer_t&>{},
                     [](timer_t& t, auto tk)
                     {
                        return t.async_wait(std::move(tk));
                     }, py::arg("self"));

    cl  .def(py::init<asio::any_io_executor>(), py::arg("executor"))
        .def(py::init<asio::io_context&>(),      py::arg("context"))
        .def(py::init<asio::system_context &>(), py::arg("context"))
        .def(py::init<asio::thread_pool &>(),    py::arg("context"))
        .def(py::init<asio::any_io_executor,  typename timer_t::time_point>(), py::arg("executor"), py::arg("expiry_time"))
        .def(py::init<asio::io_context&,      typename timer_t::time_point>(), py::arg("context"), py::arg("expiry_time"))
        .def(py::init<asio::system_context &, typename timer_t::time_point>(), py::arg("context"), py::arg("expiry_time"))
        .def(py::init<asio::thread_pool &,    typename timer_t::time_point>(), py::arg("context"), py::arg("expiry_time"))
        .def(py::init<asio::any_io_executor,  typename timer_t::duration>(), py::arg("executor"), py::arg("expiry_time"))
        .def(py::init<asio::io_context&,      typename timer_t::duration>(), py::arg("context"), py::arg("expiry_time"))
        .def(py::init<asio::system_context &, typename timer_t::duration>(), py::arg("context"), py::arg("expiry_time"))
        .def(py::init<asio::thread_pool &,    typename timer_t::duration>(), py::arg("context"), py::arg("expiry_time"))
        .def("cancel", py::overload_cast<>(&timer_t::cancel))
        .def("cancel_one", py::overload_cast<>(&timer_t::cancel_one))
        .def("expires_after", &timer_t::expires_after)
        .def("expiry",        &timer_t::expiry)
        .def("get_executor", &timer_t::get_executor)
        .def("wait", py::overload_cast<>(&timer_t::wait))
        .def("wait", py::overload_cast<asio::error_code&>(&timer_t::wait))
        ;
}


void register_steady_timer(pybind11::module_ & m);
void register_system_timer(pybind11::module_ & m);
void register_high_resolution_timer(pybind11::module_ & m);

}

#endif //ASIO_PY_TIMER_HPP
