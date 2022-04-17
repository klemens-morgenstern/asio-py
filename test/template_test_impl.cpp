// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pybind11.h>
#include <pybind11/chrono.h>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/helpers/template.hpp>

template<typename T>
struct bind_
{
};

template<typename T, typename U>
struct bind_2
{
};

template<typename T, typename U, typename R>
struct bind_3
{
};

PYBIND11_MODULE(template_test_impl, m)
{
    namespace py = pybind11;

    py::class_<bind_<int>>(m, "bind_int");
    py::class_<bind_<double>>(m, "bind_double");
    py::class_<bind_<std::string>>(m, "bind_str");
    py::class_<bind_2<int, double>>(m, "bind_int_double");
    py::class_<bind_2<double, double>>(m, "bind_double_double");
    py::class_<bind_2<int, int>>(m, "bind_int_int");
    py::class_<bind_2<double, int>>(m, "bind_double_int");
    py::class_<bind_2<int, std::string>>(m, "bind_int_str");
    py::class_<bind_2<double, std::string>>(m, "bind_double_str");
    py::class_<bind_3<double, int, std::string>>(m, "bind_double_int_str");

    m.def("static_",
          asio_py::make_static_template_picker<bind_>(asio_py::register_tag<int, double, std::string>{}),
          py::arg("type_") = asio_py::type_handle<int>());

    m.def("static_2",
          asio_py::make_static_template_picker<bind_2>(
                  asio_py::register_tag<int, double>{},
                  asio_py::register_tag<int, double, std::string>{}),
          py::arg("type_1") = asio_py::type_handle<int>(),
          py::arg("type_2") = asio_py::type_handle<double>());


    m.def("static_3",
          asio_py::make_static_template_picker<bind_3>(
                  asio_py::register_tag<double>{},
                  asio_py::register_tag<int>{},
                  asio_py::register_tag<std::string>{}),
          py::arg("type_1") = asio_py::type_handle<double>(),
          py::arg("type_2") = asio_py::type_handle<int>(),
          py::arg("type_3") = asio_py::type_handle<std::string>()
    );

    asio_py::make_dynamic_template_picker<bind_> (m, "dyn_1").add_type<double>();
    asio_py::make_dynamic_template_picker<bind_2>(m, "dyn_2").add_type<double, int>();
    asio_py::make_dynamic_template_picker<bind_3>(m, "dyn_3").add_type<double, int, std::string>();

}