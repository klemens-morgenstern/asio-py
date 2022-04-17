// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>
#include <asio-py/co_spawn.hpp>
#include <asio-py/register_all.hpp>
#include <pybind11/embed.h>
#include <pybind11/eval.h>

#include <iostream>

PYBIND11_EMBEDDED_MODULE(asio, m) {
    asio_py::register_all(m);
}

asio::awaitable<void> writer(asio::writable_pipe &wp)
{
    co_await asio::async_write(wp, asio::buffer("Line 1\n"), asio::use_awaitable);
    std::cerr << "Sent: Line 1" << std::endl;
    co_await asio::async_write(wp, asio::buffer("Line 2\n"), asio::use_awaitable);
    std::cerr << "Sent: Line 2" << std::endl;
    co_await asio::async_write(wp, asio::buffer("Line 3\n"), asio::use_awaitable);
    std::cerr << "Sent: Line 3" << std::endl;
    co_await asio::async_write(wp, asio::buffer("Line 4\n"), asio::use_awaitable);
    std::cerr << "Sent: line 4" << std::endl;
    co_await asio::async_write(wp, asio::buffer("Line 5\n"), asio::use_awaitable);
    std::cerr << "Sent: line 5" << std::endl;
}

int main(int argc, const char * argv[])
{
    asio::io_context ctx;

    namespace py = pybind11;
    py::scoped_interpreter si;

    using pybind11::literals::operator""_a;

    auto asio_py = py::module_::import("asio");
    auto glob = py::globals();
    glob["asio"] = asio_py;

    asio::readable_pipe rp{ctx};
    asio::writable_pipe wp{ctx};
    asio::connect_pipe(rp, wp);

    auto locals = py::dict();

    py::exec(R"(
print("Checking in from embed.py", asio)

def check_res(e, v):
    if e is not None:
        print(f"Error {e} {type(e)}")
    assert e is None, f"{e} is None"
    assert v == 43, f"{v} == 43"

async def reader(r):
    print("Work started", asio,  r,  await asio.this_coro.executor)
    async def check(r, cmp: str):
        print("Read")
        nx = await asio.async_read_until(r, '\n', str)
        print("Read", nx)
        assert nx == cmp, f"`{nx}` == `{cmp}`"

    await check(r, "Line 1\n")
    await check(r, "Line 2\n")
    await check(r, "Line 3\n")
    await check(r, "Line 4\n")
    await check(r, "Line 5\n")
    return 43


)", glob, locals);


    asio::co_spawn(ctx, writer(wp), asio::detached);
    asio::co_spawn(ctx, asio_py::adopt_async_func(locals["reader"](std::move(rp))),
                   [](std::exception_ptr e, py::object obj)
                   {
                        std::cerr << "Completion handler " << obj << " - "  << obj.is_none() << std::endl;
                        if (e)
                        {
                            try
                            {
                                std::rethrow_exception(e);
                            }
                            catch(std::exception & e)
                            {
                                std::cerr << "Reader Exception: " << e.what() << std::endl;
                            }
                            assert(e);
                        }

                        py::print("Res", obj);
                        assert(py::cast<int>(obj) == 43);
                   });

    ctx.run();

    return 0;
}