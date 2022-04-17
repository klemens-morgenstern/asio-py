import os

import sys

sys.path.append(os.getcwd())

import asio


ctx = asio.io_context()
exec = ctx.get_executor()


result = None

def cpl(err, x):
    global result
    result = x
    assert err is None, f"Error {err}"


async def bar():
    exec = await asio.this_coro.executor

    assert exec == ctx.get_executor(), "Comparing executors"
    assert (await asio.this_coro.cancellation_state).cancelled() == asio.cancellation_type.none, "Comparing cancel type"
    cs = asio.cancellation_signal()

    await asio.post(ctx.get_executor())
    await asio.dispatch(ctx.get_executor())
    await asio.defer(ctx.get_executor())
    return 42

assert result is None

ai = asio.any_io_executor(ctx.get_executor())
asio.co_spawn(ctx.get_executor(), bar(), cpl)
ctx.run()

assert result == 42


