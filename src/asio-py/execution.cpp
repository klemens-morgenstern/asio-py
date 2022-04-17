// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/execution.hpp>
#include <pybind11/operators.h>

namespace asio_py
{

void register_execution_properties(pybind11::module_ &m)
{
    namespace py = pybind11;
    auto ex = m.def_submodule("execution");

    py::class_<asio::execution::blocking_t> blocking(ex, "blocking_t");
    py::class_<asio::execution::blocking_t::always_t>(blocking, "always_t")
            .def(py::init<>())
            .def("value", &asio::execution::blocking_t::always_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::self == asio::execution::blocking.possibly)
            .def(py::self != asio::execution::blocking.possibly)
            .def(py::self == asio::execution::blocking.never)
            .def(py::self != asio::execution::blocking.never)
            .def_readonly_static("is_preferable", &asio::execution::blocking_t::always_t::is_preferable)
            .def_readonly_static("is_preferable", &asio::execution::blocking_t::always_t::is_requirable)
            ;


    py::class_<asio::execution::blocking_t::never_t>(blocking, "never_t")
            .def(py::init<>())
            .def("value", &asio::execution::blocking_t::never_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::self == asio::execution::blocking.always)
            .def(py::self != asio::execution::blocking.always)
            .def(py::self == asio::execution::blocking.possibly)
            .def(py::self != asio::execution::blocking.possibly)
            .def_readonly_static("is_preferable", &asio::execution::blocking_t::never_t::is_preferable)
            .def_readonly_static("is_requirable", &asio::execution::blocking_t::never_t::is_requirable)
            ;

    py::class_<asio::execution::blocking_t::possibly_t>(blocking, "possibly_t")
            .def(py::init<>())
            .def("value", &asio::execution::blocking_t::possibly_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::self == asio::execution::blocking.always)
            .def(py::self != asio::execution::blocking.always)
            .def(py::self == asio::execution::blocking.never)
            .def(py::self != asio::execution::blocking.never)
            .def_readonly_static("is_preferable", &asio::execution::blocking_t::possibly_t::is_preferable)
            .def_readonly_static("is_requirable", &asio::execution::blocking_t::possibly_t::is_requirable)
            ;

    
    blocking.def(py::init<>())
            .def(py::init<asio::execution::blocking_t::possibly_t>())
            .def(py::init<asio::execution::blocking_t::always_t>())
            .def(py::init<asio::execution::blocking_t::never_t>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("possibly", &asio::execution::blocking.possibly)
            .def_readonly_static("always", &asio::execution::blocking.always)
            .def_readonly_static("never", &asio::execution::blocking.never)
            ;

    ex.attr("blocking") = asio::execution::blocking;
    blocking.attr("polymorphic_query_result_type") = py::type::of<asio::execution::blocking_t>();

    py::class_<asio::execution::outstanding_work_t> outstanding_work(ex, "outstanding_work_t");

    py::class_<asio::execution::outstanding_work_t::tracked_t>(outstanding_work, "tracked_t")
            .def(py::init<>())
            .def("value", &asio::execution::outstanding_work_t::tracked_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("is_preferable", &asio::execution::outstanding_work_t::tracked_t::is_preferable)
            .def_readonly_static("is_preferable", &asio::execution::outstanding_work_t::tracked_t::is_requirable)
            ;


    py::class_<asio::execution::outstanding_work_t::untracked_t>(outstanding_work, "untracked_t")
            .def(py::init<>())
            .def("value", &asio::execution::outstanding_work_t::untracked_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("is_preferable", &asio::execution::outstanding_work_t::untracked_t::is_preferable)
            .def_readonly_static("is_requirable", &asio::execution::outstanding_work_t::untracked_t::is_requirable)
            ;

    outstanding_work.def(py::init<>())
            .def(py::init<asio::execution::outstanding_work_t::tracked_t>())
            .def(py::init<asio::execution::outstanding_work_t::untracked_t>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("tracked", &asio::execution::outstanding_work.tracked)
            .def_readonly_static("untracked", &asio::execution::outstanding_work.untracked)
            ;

    outstanding_work.attr("polymorphic_query_result_type") = py::type::of<asio::execution::outstanding_work_t>();
    ex.attr("outstanding_work") = asio::execution::outstanding_work;


    py::class_<asio::execution::relationship_t> relationship(ex, "relationship_t");

    py::class_<asio::execution::relationship_t::fork_t>(relationship, "fork_t")
            .def(py::init<>())
            .def("value", &asio::execution::relationship_t::fork_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("is_preferable", &asio::execution::relationship_t::fork_t::is_preferable)
            .def_readonly_static("is_preferable", &asio::execution::relationship_t::fork_t::is_requirable)
            ;


    py::class_<asio::execution::relationship_t::continuation_t>(relationship, "continuation_t")
            .def(py::init<>())
            .def("value", &asio::execution::relationship_t::continuation_t::value)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("is_preferable", &asio::execution::relationship_t::continuation_t::is_preferable)
            .def_readonly_static("is_requirable", &asio::execution::relationship_t::continuation_t::is_requirable)
            ;

    relationship.def(py::init<>())
            .def(py::init<asio::execution::relationship_t::fork_t>())
            .def(py::init<asio::execution::relationship_t::continuation_t>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def_readonly_static("fork", &asio::execution::relationship.fork)
            .def_readonly_static("continuation", &asio::execution::relationship.continuation)
            ;

    relationship.attr("polymorphic_query_result_type") = py::type::of<asio::execution::relationship_t>();
    ex.attr("relationship") = asio::execution::relationship;
}

}