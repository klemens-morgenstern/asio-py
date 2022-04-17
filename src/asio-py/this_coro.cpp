// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio/awaitable.hpp>
#include <asio-py/awaitable.hpp>

namespace asio_py
{

void register_this_coro(pybind11::module_ & parent)
{
    namespace py = pybind11;

    py::dict locals = py::dict();
    py::exec(R"(def __await_impl(self):
    res = yield self
    return res)", py::globals(), locals);
    py::function await_impl = locals["__await_impl"];

    auto m = parent.def_submodule("this_coro");

    py::class_<asio::this_coro::executor_t>(m, "executor_t")
            .def(py::init<>())
            .attr("__await__") = await_impl;
            ;
    m.attr("executor") = asio::this_coro::executor;

    py::class_<asio::this_coro::cancellation_state_t>(m, "cancellation_state_t")
            .def(py::init<>())
            .attr("__await__") = await_impl;
            ;

    m.attr("cancellation_state") = asio::this_coro::cancellation_state;

    using f = std::function<asio::cancellation_type(asio::cancellation_type)>;
    
    py::class_<asio::this_coro::reset_cancellation_state_0_t>(m, "reset_cancellation_state_0_t")
            .def(py::init<>())
            .attr("__await__") = await_impl;
            ;

    py::class_<asio::this_coro::reset_cancellation_state_1_t<f>>(m, "reset_cancellation_state_1_t")
            .def(py::init<f>(), py::arg("filter"))
            .attr("__await__") = await_impl;

            ;
    py::class_<asio::this_coro::reset_cancellation_state_2_t<f, f>>(m, "reset_cancellation_state_2_t")
            .def(py::init<f, f>(), py::arg("in_filter"), py::arg("out_filter"))
            .attr("__await__") = await_impl;
            ;

            
    m.def("reset_cancellation_state", [] {return asio::this_coro::reset_cancellation_state(); });
    m.def("reset_cancellation_state", [](f func) {return asio::this_coro::reset_cancellation_state(func, func); }, py::arg("filter"));
    m.def("reset_cancellation_state", [](f in, f out) {return asio::this_coro::reset_cancellation_state(std::move(in), std::move(out)); }, py::arg("in_filter"), py::arg("out_filter"));

    py::class_<asio::this_coro::throw_if_cancelled_0_t>(m, "throw_if_cancelled_0_t")
            .def(py::init<>())
            .attr("__await__") = await_impl;
            ;
    py::class_<asio::this_coro::throw_if_cancelled_1_t>(m, "throw_if_cancelled_1_t")
            .def(py::init<bool>(), py::arg("value"))
            .attr("__await__") = await_impl;
            ;

    m.def("throw_if_cancelled", py::overload_cast<>(&asio::this_coro::throw_if_cancelled));
    m.def("throw_if_cancelled", py::overload_cast<bool>(&asio::this_coro::throw_if_cancelled), py::arg("value"));
}

}