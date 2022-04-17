// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_ANY_IO_EXECUTOR_HPP
#define ASIO_PY_ANY_IO_EXECUTOR_HPP

#include <asio-py/helpers/options.hpp>
#include <asio-py/helpers/tag.hpp>
#include <pybind11.h>
#include <operators.h>

namespace asio_py
{

namespace detail
{

template<typename Executor>
pybind11::object target_impl(const asio::any_io_executor & exec)
{
    if constexpr (std::is_same_v<Executor, asio::any_io_executor>)
        return pybind11::cast(exec);
    else
    {
        auto t = exec.target<Executor>();
        if (t == nullptr)
            return pybind11::none();
        else
            return pybind11::cast(*t);
    }
}


template< typename ... Executors>
void register_executors(pybind11::class_<asio::any_io_executor> & c, register_tag<Executors...>)
{
    namespace py = pybind11;

    (c.def(py::init<Executors>()), ...);
    (py::implicitly_convertible<Executors, asio::any_io_executor>(), ...);
    c.def("target",
          [
              tps = std::array<py::type, sizeof...(Executors)>{py::type::of<Executors>()...},
              cst = std::array<py::object(*)(const asio::any_io_executor &), sizeof...(Executors)>{&target_impl<Executors>...}
          ](const asio::any_io_executor & exec, py::object o) -> py::object
          {
              auto t = o.template cast< py::type >();
              auto itr = std::find(tps.begin(), tps.end(), t);
              if (itr == tps.end())
                  throw py::type_error();
              return cst[std::distance(tps.begin(), itr)](exec);
          });
}

}

inline pybind11::class_< asio::any_io_executor > declare_any_io_executor(pybind11::module & m)
{
    asio::any_io_executor exec;
    namespace py = pybind11;
    py::class_< asio::any_io_executor > aie(m, "any_io_executor");
    aie .def(py::init<>())
        .def(py::init<std::nullptr_t>())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("swap", &asio::any_io_executor::swap)
        ;

    return aie;
}

}

#endif //ASIO_PY_ANY_IO_EXECUTOR_HPP
