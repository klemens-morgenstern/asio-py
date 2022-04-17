// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_STRAND_HPP
#define ASIO_PY_STRAND_HPP

#include <asio-py/helpers/options.hpp>

#include <pybind11.h>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/helpers/template.hpp>
#include <asio-py/detail/typename.hpp>

namespace asio_py
{

template<typename Executor>
void register_strand_impl(pybind11::object & m, register_tag<Executor>)
{
    namespace py = pybind11;

    using tp = asio::strand<Executor>;
    const auto nm = "__" + detail::get_sanitized_type_name<tp>();
    py::class_<tp>(m, nm.c_str())
            .def("context", &tp::context, "Obtain the underlying execution context.")
            .def("get_inner_executor", &tp::get_inner_executor, "Obtain the underlying executor.")
            .def("on_work_finished", &tp::on_work_finished, "Inform the strand that some work is no longer outstanding.")
            .def("on_work_started", &tp::on_work_started, "Inform the strand that it has some outstanding work to do.")
            .def("running_in_this_thread", &tp::running_in_this_thread, "Determine whether the strand is running in the current thread.")
            .attr("inner_executor_type") = py::type::of<typename tp::inner_executor_type>()
            ;
}

void register_strand_impl(pybind11::object & m, register_tag<asio::any_io_executor>)
{
    namespace py = pybind11;

    using tp = asio::strand<asio::any_io_executor>;
    const auto nm = "__" + detail::get_sanitized_type_name<tp>();
    py::class_<tp>(m, nm.c_str())
            .def("context", &tp::context, "Obtain the underlying execution context.")
            .def("get_inner_executor", &tp::get_inner_executor, "Obtain the underlying executor.")
            .def("running_in_this_thread", &tp::running_in_this_thread, "Determine whether the strand is running in the current thread.")
            .attr("inner_executor_type") = py::type::of<typename tp::inner_executor_type>()
            ;
}


template<typename AllExecutors = all_executors>
void register_strand(pybind11::module_ & m)
{
    namespace py = pybind11;
    using regs_t = AllExecutors;
    const auto f = make_static_template_picker<asio::strand>(regs_t{});

    struct never_t {};
    pybind11::class_<never_t> ccl(m, "__strand_t");
    m.add_object("strand", py::cast(never_t()));

    [&]<typename ... Regs>(register_tag<Regs...> tt)
    {
        (register_strand_impl(ccl, register_tag<Regs>{}), ...);
    }(regs_t{});

    ccl.def("__getitem__",
            [f](const never_t &, py::object obj)
            {
                const auto tp = obj.cast<py::type>();
                return f(std::move(tp));
            });

}

}

#endif //ASIO_PY_STRAND_HPP
