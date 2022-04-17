// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_PIPE_HPP
#define ASIO_PY_PIPE_HPP

#include <asio/connect_pipe.hpp>
#include <asio/readable_pipe.hpp>
#include <asio/writable_pipe.hpp>
#include <asio/thread_pool.hpp>
#include <asio/experimental/deferred.hpp>

#include <asio-py/completion_token.hpp>
#include <asio-py/detail/typename.hpp>
#include <asio-py/helpers/template.hpp>
#include <asio-py/read.hpp>
#include <asio-py/write.hpp>

#include <pybind11.h>
#include <asio/post.hpp>

namespace asio_py
{

template<typename Executor1 = asio::any_io_executor,
         typename Executor2 = asio::any_io_executor>
void register_connect_pipe(pybind11::module_ & m)
{
    namespace py = pybind11;
    m.def("connect_pipe", [](asio::basic_readable_pipe< Executor1 > & l,
                             asio::basic_writable_pipe< Executor2 > & r) {return asio::connect_pipe(l, r);});
    m.def("connect_pipe", [](asio::basic_readable_pipe< Executor1 > & l,
                             asio::basic_writable_pipe< Executor2 > & r,
                             asio::error_code & ec) {return asio::connect_pipe(l, r, ec);});
}

template <typename Executor = asio::any_io_executor>
void register_basic_readable_pipe(pybind11::module_ & m,
                            const std::string &name = detail::get_sanitized_type_name<asio::basic_readable_pipe<Executor>>())
{
    namespace py = pybind11;

    using pt = asio::basic_readable_pipe<Executor>;
    py::class_<pt> cl(m, name.c_str());

    cl.attr("executor_type") = py::type::of<Executor>();
    cl.attr("lowest_layer_type") = cl.get_type();
    cl.attr("native_handle_type") = type_handle<typename pt::native_handle_type>();

    cl.def("assign", py::overload_cast<const typename pt::native_handle_type &>(&pt::assign), "Assign an existing native pipe to the pipe.");
    cl.def("assign", py::overload_cast<const typename pt::native_handle_type &, asio::error_code &>(&pt::assign), "Assign an existing native pipe to the pipe.");

    cl  .def(py::init<asio::any_io_executor>(), py::arg("executor"), "Construct a basic_readable_pipe without opening it.")
        .def(py::init<asio::io_context&>(),      py::arg("context"), "Construct a basic_readable_pipe without opening it.")
        .def(py::init<asio::system_context &>(), py::arg("context"), "Construct a basic_readable_pipe without opening it.")
        .def(py::init<asio::thread_pool &>(),    py::arg("context"), "Construct a basic_readable_pipe without opening it.")
        .def(py::init<asio::any_io_executor, typename pt::native_handle_type>(), py::arg("executor"), py::arg("native_pipe"), "Construct a basic_readable_pipe on an existing native pipe.")
        .def(py::init<asio::io_context&, typename pt::native_handle_type>(),      py::arg("context"), py::arg("native_pipe"), "Construct a basic_readable_pipe on an existing native pipe.")
        .def(py::init<asio::system_context &, typename pt::native_handle_type>(), py::arg("context"), py::arg("native_pipe"), "Construct a basic_readable_pipe on an existing native pipe.")
        .def(py::init<asio::thread_pool &, typename pt::native_handle_type>(),    py::arg("context"), py::arg("native_pipe"), "Construct a basic_readable_pipe on an existing native pipe.")
        .def("cancel", py::overload_cast<>(&pt::cancel), "Cancel all asynchronous operations associated with the pipe.")
        .def("cancel", py::overload_cast<asio::error_code&>(&pt::cancel), "Cancel all asynchronous operations associated with the pipe.")
        .def("close", py::overload_cast<>(&pt::close), "Close the pipe.")
        .def("close", py::overload_cast<asio::error_code&>(&pt::close), "Close the pipe.")
        .def("get_executor", &pt::get_executor, "Get the executor associated with the object.")
        .def("is_open", &pt::is_open, "Determine whether the pipe is open.")
        .def("lowest_layer",  py::overload_cast<>(&pt::lowest_layer), "Get a reference to the lowest layer.")
        .def("lowest_layer",  py::overload_cast<>(&pt::lowest_layer, py::const_), "Get a reference to the lowest layer.")
        .def("native_handle", &pt::native_handle, "Get the native pipe representation.")
        ;

    register_read(m, cl);
}

template <typename Executor = asio::any_io_executor>
void register_basic_writable_pipe(pybind11::module_ & m,
                                  const std::string &name = detail::get_sanitized_type_name<asio::basic_writable_pipe<Executor>>())
{
    namespace py = pybind11;

    using pt = asio::basic_writable_pipe<Executor>;
    py::class_<pt> cl(m, name.c_str());

    cl.attr("executor_type") = py::type::of<Executor>();
    cl.attr("lowest_layer_type") = cl.get_type();
    cl.attr("native_handle_type") = type_handle<typename pt::native_handle_type>();

    cl.def("assign", py::overload_cast<const typename pt::native_handle_type &>(&pt::assign), "Assign an existing native pipe to the pipe.");
    cl.def("assign", py::overload_cast<const typename pt::native_handle_type &, asio::error_code &>(&pt::assign), "Assign an existing native pipe to the pipe.");

    cl  .def(py::init<asio::any_io_executor>(), py::arg("executor"), "Construct a basic_writable_pipe without opening it.")
            .def(py::init<asio::io_context&>(),      py::arg("context"), "Construct a basic_writable_pipe without opening it.")
            .def(py::init<asio::system_context &>(), py::arg("context"), "Construct a basic_writable_pipe without opening it.")
            .def(py::init<asio::thread_pool &>(),    py::arg("context"), "Construct a basic_writable_pipe without opening it.")
            .def(py::init<asio::any_io_executor, typename pt::native_handle_type>(), py::arg("executor"), py::arg("native_pipe"), "Construct a basic_writable_pipe on an existing native pipe.")
            .def(py::init<asio::io_context&, typename pt::native_handle_type>(),      py::arg("context"), py::arg("native_pipe"), "Construct a basic_writable_pipe on an existing native pipe.")
            .def(py::init<asio::system_context &, typename pt::native_handle_type>(), py::arg("context"), py::arg("native_pipe"), "Construct a basic_writable_pipe on an existing native pipe.")
            .def(py::init<asio::thread_pool &, typename pt::native_handle_type>(),    py::arg("context"), py::arg("native_pipe"), "Construct a basic_writable_pipe on an existing native pipe.")
            .def("cancel", py::overload_cast<>(&pt::cancel), "Cancel all asynchronous operations associated with the pipe.")
            .def("cancel", py::overload_cast<asio::error_code&>(&pt::cancel), "Cancel all asynchronous operations associated with the pipe.")
            .def("close", py::overload_cast<>(&pt::close), "Close the pipe.")
            .def("close", py::overload_cast<asio::error_code&>(&pt::close), "Close the pipe.")
            .def("get_executor", &pt::get_executor, "Get the executor associated with the object.")
            .def("is_open", &pt::is_open, "Determine whether the pipe is open.")
            .def("lowest_layer",  py::overload_cast<>(&pt::lowest_layer), "Get a reference to the lowest layer.")
            .def("lowest_layer",  py::overload_cast<>(&pt::lowest_layer, py::const_), "Get a reference to the lowest layer.")
            .def("native_handle", &pt::native_handle, "Get the native pipe representation.")
            ;

    register_write(m, cl);
}

void register_readable_pipe(pybind11::module_ & m);
void register_writable_pipe(pybind11::module_ & m);

}

#endif //ASIO_PY_PIPE_HPP
