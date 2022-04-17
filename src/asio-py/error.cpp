// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/error.hpp>
#include <asio/error.hpp>
#include <operators.h>


namespace asio_py
{

void register_error(pybind11::module_ &m)
{
    namespace py = pybind11;

    py::class_<asio::error_code>(m, "error_code")
            .def("assign", &asio::error_code::assign)
            .def("value", &asio::error_code::value)
            .def("clear", &asio::error_code::clear)
            .def("message", &asio::error_code::message)
            .def("category", &asio::error_code::category)
            .def("default_error_condition", &asio::error_code::default_error_condition)
            .def("__str__", &asio::error_code::message)
            .def(!py::self);

    py::class_<asio::error_category>(m, "error_category")
            .def("default_error_condition", &asio::error_category::default_error_condition)
            .def("message", &asio::error_category::message)
            .def("name", &asio::error_category::name)
            .def("equivalent",
                 py::overload_cast<const asio::error_code &, int>(&asio::error_category::equivalent, py::const_))
            .def(py::self != py::self)
            .def(py::self == py::self)
            .def(py::self < py::self);

    auto err = m.def_submodule("error");

    py::enum_<asio::error::basic_errors>(err, "basic_errors")
            .value("access_denied", asio::error::access_denied, "Permission denied.")
            .value("address_family_not_supported", asio::error::address_family_not_supported, "Address family not supported by protocol.")
            .value("address_in_use", asio::error::address_in_use, "Address already in use.")
            .value("already_connected", asio::error::already_connected, "Transport endpoint is already connected.")
            .value("already_started", asio::error::already_started, "Operation already in progress.")
            .value("broken_pipe", asio::error::broken_pipe, "Broken pipe.")
            .value("connection_aborted", asio::error::connection_aborted, "A connection has been aborted.")
            .value("connection_refused", asio::error::connection_refused, "Connection refused.")
            .value("connection_reset", asio::error::connection_reset, "Connection reset by peer.")
            .value("bad_descriptor", asio::error::bad_descriptor, "Bad file descriptor.")
            .value("fault", asio::error::fault, "Bad address.")
            .value("host_unreachable", asio::error::host_unreachable, "No route to host.")
            .value("in_progress", asio::error::in_progress, "Operation now in progress.")
            .value("interrupted", asio::error::interrupted, "Interrupted system call.")
            .value("invalid_argument", asio::error::invalid_argument, "Invalid argument.")
            .value("message_size", asio::error::message_size, "Message too long.")
            .value("name_too_long", asio::error::name_too_long, "The name was too long.")
            .value("network_down", asio::error::network_down, "Network is down.")
            .value("network_reset", asio::error::network_reset, "Network dropped connection on reset.")
            .value("network_unreachable", asio::error::network_unreachable, "Network is unreachable.")
            .value("no_descriptors", asio::error::no_descriptors, "Too many open files.")
            .value("no_buffer_space", asio::error::no_buffer_space, "No buffer space available.")
            .value("no_memory", asio::error::no_memory, "Cannot allocate memory.")
            .value("no_permission", asio::error::no_permission, "Operation not permitted.")
            .value("no_protocol_option", asio::error::no_protocol_option, "Protocol not available.")
            .value("no_such_device", asio::error::no_such_device, "No such device.")
            .value("not_connected", asio::error::not_connected, "Transport endpoint is not connected.")
            .value("not_socket", asio::error::not_socket, "Socket operation on non-socket.")
            .value("operation_aborted", asio::error::operation_aborted, "Operation cancelled.")
            .value("operation_not_supported", asio::error::operation_not_supported, "Operation not supported.")
            .value("shut_down", asio::error::shut_down, "Cannot send after transport endpoint shutdown.")
            .value("timed_out", asio::error::timed_out, "Connection timed out.")
            .value("try_again", asio::error::try_again, "Resource temporarily unavailable.")
            .value("would_block", asio::error::would_block,  "The socket is marked non-blocking and the requested operation would block.");

    err.attr("access_denied") =  asio::error::access_denied;
    err.attr("address_family_not_supported") =  asio::error::address_family_not_supported;
    err.attr("address_in_use") =  asio::error::address_in_use;
    err.attr("already_connected") =  asio::error::already_connected;
    err.attr("already_started") =  asio::error::already_started;
    err.attr("broken_pipe") =  asio::error::broken_pipe;
    err.attr("connection_aborted") =  asio::error::connection_aborted;
    err.attr("connection_refused") =  asio::error::connection_refused;
    err.attr("connection_reset") =  asio::error::connection_reset;
    err.attr("bad_descriptor") =  asio::error::bad_descriptor;
    err.attr("fault") =  asio::error::fault;
    err.attr("host_unreachable") =  asio::error::host_unreachable;
    err.attr("in_progress") =  asio::error::in_progress;
    err.attr("interrupted") =  asio::error::interrupted;
    err.attr("invalid_argument") =  asio::error::invalid_argument;
    err.attr("message_size") =  asio::error::message_size;
    err.attr("name_too_long") =  asio::error::name_too_long;
    err.attr("network_down") =  asio::error::network_down;
    err.attr("network_reset") =  asio::error::network_reset;
    err.attr("network_unreachable") =  asio::error::network_unreachable;
    err.attr("no_descriptors") =  asio::error::no_descriptors;
    err.attr("no_buffer_space") =  asio::error::no_buffer_space;
    err.attr("no_memory") =  asio::error::no_memory;
    err.attr("no_permission") =  asio::error::no_permission;
    err.attr("no_protocol_option") =  asio::error::no_protocol_option;
    err.attr("no_such_device") =  asio::error::no_such_device;
    err.attr("not_connected") =  asio::error::not_connected;
    err.attr("not_socket") =  asio::error::not_socket;
    err.attr("operation_aborted") =  asio::error::operation_aborted;
    err.attr("operation_not_supported") =  asio::error::operation_not_supported;
    err.attr("shut_down") =  asio::error::shut_down;
    err.attr("timed_out") =  asio::error::timed_out;
    err.attr("try_again") =  asio::error::try_again;
    err.attr("would_block") =  asio::error::would_block;


    py::implicitly_convertible<asio::error::basic_errors, asio::error_code>();


    py::enum_<asio::error::netdb_errors>(err, "netdb_errors")
            .value("host_not_found",            asio::error::host_not_found, "Host not found (authoritative).")
            .value("host_not_found_try_again",  asio::error::host_not_found_try_again, "Host not found (non-authoritative).")
            .value("no_data",                   asio::error::no_data, "The query is valid but does not have associated address data.")
            .value("no_recovery",               asio::error::no_recovery, "A non-recoverable error occurred.")
                    ;

    err.attr("host_not_found")           = asio::error::host_not_found;
    err.attr("host_not_found_try_again") = asio::error::host_not_found_try_again;
    err.attr("no_data")                  = asio::error::no_data;
    err.attr("no_recovery")              = asio::error::no_recovery;

    py::implicitly_convertible<asio::error::netdb_errors, asio::error_code>();

    py::enum_<asio::error::addrinfo_errors>(err, "addrinfo_errors")
            .value("service_not_found", asio::error::service_not_found, "The service is not supported for the given socket type.") 
            .value("socket_type_not_supported", asio::error::socket_type_not_supported, "The socket type is not supported.") 
    ;

    err.attr("service_not_found")         = asio::error::service_not_found;
    err.attr("socket_type_not_supported") = asio::error::socket_type_not_supported;

    py::implicitly_convertible<asio::error::addrinfo_errors, asio::error_code>();

    py::enum_<asio::error::misc_errors>(err, "misc_errors")
            .value("already_open", asio::error::already_open, "Already open.")
            .value("eof", asio::error::eof, "End of file or stream.")
            .value("not_found", asio::error::not_found, "Element not found.")
            .value("fd_set_failure", asio::error::fd_set_failure, "The descriptor cannot fit into the select system call's fd_set.")
        ;

    err.attr("already_open")   =  asio::error::already_open;
    err.attr("eof")            =  asio::error::eof;
    err.attr("not_found")      =  asio::error::not_found;
    err.attr("fd_set_failure") =  asio::error::fd_set_failure;
    py::implicitly_convertible<asio::error::misc_errors, asio::error_code>();

    err.def("get_system_category",   &asio::error::get_system_category);
    err.def("get_netdb_category",    &asio::error::get_netdb_category);
    err.def("get_addrinfo_category", &asio::error::get_addrinfo_category);
    err.def("get_misc_category",     &asio::error::get_misc_category);
}

}