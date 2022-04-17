// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_PREFER_HPP
#define ASIO_PY_PREFER_HPP

#include <asio/prefer.hpp>
#include <asio-py/helpers/options.hpp>

namespace asio_py
{

template<typename Executor, typename Property>
void register_prefer(pybind11::module_ & m, register_tag<Executor> = {}, register_tag<Property> = {})
{
    if constexpr (asio::can_prefer_v<Executor, Property>)
        m.def("prefer", +[](const Executor & exec, const Property& prop) { return asio::prefer(exec, prop);});
}

template<typename ... Executors, typename ... Properties>
void register_prefer(pybind11::module_ & m, register_tag<Executors...>, register_tag<Properties...>)
{
    auto inner =
        [&]<typename Exec>(register_tag<Exec> tag)
        {
            (register_prefer(m, tag, register_tag<Properties>{}), ...);
        };
    (inner(register_tag<Executors>{}), ...);
}

}

#endif //ASIO_PY_PREFER_HPP
