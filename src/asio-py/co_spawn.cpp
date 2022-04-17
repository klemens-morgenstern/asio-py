// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/co_spawn.hpp>

namespace asio_py
{

asio::awaitable<pybind11::object> adopt_async_func(pybind11::object obj)
{
    namespace py = pybind11;

    if (!hasattr(obj, "__await__") && hasattr(obj, "__call__"))
        obj = obj();


    const auto exec = co_await asio::this_coro::executor;
    const auto cr = obj.attr("__await__")();
    try
    {
        auto nx = cr.attr("__next__");

        py::object in = py::none();
        while (true)
        {
            using f = std::function<asio::cancellation_type(asio::cancellation_type)>;
            auto y = in.is_none() ? cr.attr("__next__")() : cr.attr("send")(in);

            in = py::none();
            if (py::isinstance<awaitable_wrapper>(y))
                in = co_await std::move(py::cast<awaitable_wrapper&>(y)).await();
            else if (py::isinstance<asio::this_coro::executor_t>(y))
                in = py::cast(co_await asio::this_coro::executor);
            else if (py::isinstance<asio::this_coro::cancellation_state_t>(y))
                in = py::cast(co_await asio::this_coro::cancellation_state);
            else if (py::isinstance<asio::this_coro::reset_cancellation_state_0_t>(y))
                co_await py::cast<asio::this_coro::reset_cancellation_state_0_t>(std::move(y));
            else if (py::isinstance<asio::this_coro::reset_cancellation_state_2_t<f, f>>(y))
                co_await py::cast<asio::this_coro::reset_cancellation_state_2_t<f, f>>(std::move(y));
            else if (py::isinstance<asio::this_coro::throw_if_cancelled_0_t>(y))
                in = py::cast(co_await py::cast<asio::this_coro::throw_if_cancelled_0_t>(std::move(y)));
            else if (py::isinstance<asio::this_coro::throw_if_cancelled_1_t>(y))
                co_await py::cast<asio::this_coro::throw_if_cancelled_1_t>(std::move(y));
            else
                throw std::logic_error("Unsupported object awaited in co_spawn " + py::cast<std::string>(py::str(y.get_type())));

        }
    }
    catch(py::stop_iteration & si)
    {
    }
    catch (py::error_already_set & e)
    {

        if (py::cast<std::string>(py::str(e.type())) != "<class 'StopIteration'>")
            throw;
        else
            co_return e.value().attr("value");
    }
    co_return py::none();
}

}