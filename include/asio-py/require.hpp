// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_REQUIRE_HPP
#define ASIO_PY_REQUIRE_HPP

#include <asio/require.hpp>
#include <asio-py/helpers/options.hpp>

namespace asio_py
{

template<typename Executor, typename Property>
void register_require(pybind11::module_ & m, register_tag<Executor> = {}, register_tag<Property> = {})
{
    if constexpr (asio::can_require_v<Executor, Property>)
        m.def("require", +[](const Executor & exec, const Property& prop) { return asio::require(exec, prop);});
}

template<typename ... Executors, typename ... Properties>
void register_require(pybind11::module_ & m, register_tag<Executors...>, register_tag<Properties...>)
{
    auto inner =
            [&]<typename Exec>(register_tag<Exec> tag)
            {
                (register_require(m, tag, register_tag<Properties>{}), ...);
            };
    (inner(register_tag<Executors>{}), ...);
}

}

#endif //ASIO_PY_REQUIRE_HPP
