import os
import sys

sys.path.append(os.getcwd())

import asio

ctx = asio.io_context()

rp = asio.readable_pipe(ctx)
wp = asio.writable_pipe(ctx)
asio.connect_pipe(rp, wp)

class buffer:
    buffer_impl : str

async def reader(r):
    async def check(r, cmp: str):
        nx = await asio.async_read_until(r, '\n', str)
        print("Read", nx)
        assert nx == cmp, f"`{nx}` == `{cmp}`"

    await check(r, "LINE 1\n")
    await check(r, "LINE 2\n")
    await check(r, "LINE 3\n")
    await check(r, "LINE 4\n")
    await check(r, "LINE 5\n")


async def writer(w):
    print("Write, Line 1")
    await asio.async_write(w, "LINE 1\n")
    print("Write, Line 2")
    await asio.async_write(w, "LINE 2\n")
    print("Write, Line 3")
    await asio.async_write(w, "LINE 3\n")
    print("Write, Line 4")
    await asio.async_write(w, "LINE 4\n")
    print("Write, Line 5")
    await asio.async_write(w, "LINE 5\n")
    print("Done")


def check_res(e):
    if e is not None:
        print(f"Error {e} {type(e)}")
    assert e == None


asio.co_spawn(ctx.get_executor(), writer(wp), check_res)
asio.co_spawn(ctx.get_executor(), reader(rp), check_res)

ctx.run()
