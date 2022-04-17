// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio-py/awaitable.hpp>
#include <pybind11.h>
#include <pybind11/chrono.h>

#include <asio-py/cancellation_type.hpp>
#include <asio-py/cancellation_signal.hpp>
#include <asio-py/cancellation_state.hpp>
#include <asio-py/execution_context.hpp>
#include <asio-py/io_context.hpp>
#include <asio-py/co_spawn.hpp>
#include <asio-py/this_coro.hpp>
#include <asio-py/use_awaitable.hpp>

#include <asio-py/helpers/options.hpp>
#include "asio-py/any_io_executor.hpp"
#include "asio-py/thread_pool.hpp"
#include "asio-py/system_context.hpp"
#include "asio-py/use_future.hpp"
#include "asio-py/prefer.hpp"
#include "asio-py/strand.hpp"
#include "asio-py/query.hpp"
#include "asio-py/require.hpp"
#include "asio-py/execution.hpp"
#include "asio-py/post.hpp"
#include "asio-py/register_all.hpp"
#include "asio-py/detached.hpp"
#include "asio-py/defer.hpp"
#include "asio-py/dispatch.hpp"
#include "asio-py/timer.hpp"
#include "asio-py/pipe.hpp"
#include "asio-py/error.hpp"

void asio_py::register_all(pybind11::module_ & m)
{
    namespace py = pybind11;

    asio_py::register_execution_context(m);

    asio_py::register_io_context(m);
    asio_py::register_system_context(m);
    asio_py::register_thread_pool(m);

    auto aio = asio_py::declare_any_io_executor(m);
    asio_py::register_strand(m);

    asio_py::register_execution_properties(m);

    asio_py::register_prefer(m, asio_py::all_executors_with_strands{}, asio_py::executor_properties{});
    asio_py::register_query(m, asio_py::all_executors_with_strands{}, asio_py::executor_properties{});
    asio_py::register_require(m, asio_py::all_executors_with_strands{}, asio_py::executor_properties{});

    asio_py::register_use_awaitable(m);
    asio_py::register_use_future(m);

    asio_py::register_cancellation_type(m);
    asio_py::register_cancellation_signal(m);
    asio_py::register_cancellation_state(m);

    asio_py::detail::register_executors(aio, asio_py::all_executors_with_strands{});


    asio_py::register_this_coro(m);
    asio_py::register_awaitable(m);
    asio_py::register_detached(m);

    asio_py::register_co_spawn(m);

    asio_py::register_defer(m);
    asio_py::register_dispatch(m);
    asio_py::register_post(m);

    asio_py::register_steady_timer(m);
    asio_py::register_system_timer(m);
    asio_py::register_high_resolution_timer(m);

    asio_py::register_readable_pipe(m);
    asio_py::register_writable_pipe(m);
    asio_py::register_connect_pipe(m);

   asio_py::register_error(m);

}