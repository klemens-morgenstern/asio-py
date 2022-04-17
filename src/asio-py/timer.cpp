// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/timer.hpp>
#include <asio/deadline_timer.hpp>
#include <asio/high_resolution_timer.hpp>
#include <asio/steady_timer.hpp>
#include <asio/system_timer.hpp>

namespace asio_py
{
void register_steady_timer(pybind11::module_ & m)
{
    asio_py::register_basic_waitable_timer<
        asio::steady_timer::clock_type,
        asio::steady_timer::traits_type,
        asio::steady_timer::executor_type>(m, "steady_timer");
}

void register_system_timer(pybind11::module_ & m)
{
    asio_py::register_basic_waitable_timer<
            asio::system_timer::clock_type,
            asio::system_timer::traits_type,
            asio::system_timer::executor_type>(m, "system_timer");
}

void register_high_resolution_timer(pybind11::module_ & m)
{
    if constexpr (std::is_same_v<asio::high_resolution_timer, asio::steady_timer>)
        m.attr("high_resolution_timer") = pybind11::type::of<asio::steady_timer>();
    else
        asio_py::register_basic_waitable_timer<
                asio::high_resolution_timer::clock_type,
                asio::high_resolution_timer::traits_type,
                asio::high_resolution_timer::executor_type>(m, "high_resolution_timer");
}

}