// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_WRITE_HPP
#define ASIO_PY_WRITE_HPP

#include <pybind11.h>
#include <asio/write.hpp>

#include <asio-py/helpers/tag.hpp>

namespace asio_py
{


template<typename Cl>
void register_write(pybind11::module_ & m, pybind11::class_<Cl> & cl)
{
    using pt = Cl;
    namespace py = pybind11;


    cl.def("write_some", [](pt & p, py::str s)
            {
                return p.write_some(asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())));
            },
            py::arg("buffer"));
    cl.def("write_some", [](pt & p, py::buffer b)
            {
                auto bi = b.request();
                return p.write_some(asio::buffer(bi.ptr, bi.size));
            }, py::arg("buffer"));

    cl.def("write_some", [](pt & p, py::bytes b)
    {
        char *buffer = nullptr;
        ssize_t length = 0;
        if (PyBytes_AsStringAndSize(b.ptr(), &buffer, &length))
            return p.write_some(asio::buffer(buffer, length));
        else
            throw std::runtime_error("Can't read size of buffer");
    }, py::arg("buffer"));

    cl.def("write_some", [](pt & p, py::str s, asio::error_code & ec)
            {
                return p.write_some(asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())), ec);
            }, py::arg("buffer"), py::arg("ec"));
    cl.def("write_some", [](pt & p, py::buffer b, asio::error_code & ec)
    {
        auto bi = b.request();
        return p.write_some(asio::buffer(bi.ptr, bi.size), ec);
    }, py::arg("buffer"), py::arg("ec"));


    cl.def("write_some", [](pt & p, py::bytes b, asio::error_code & ec)
    {
        char *buffer = nullptr;
        ssize_t length = 0;
        if (PyBytes_AsStringAndSize(b.ptr(), &buffer, &length))
            return p.write_some(asio::buffer(buffer, length), ec);
        else
            throw std::runtime_error("Can't read size of buffer");
    }, py::arg("buffer"), py::arg("ec"));

    register_async_op(cl, "async_write_some",
                      register_tag<pt&, py::str>{},
                      [](pt& p, py::str s, auto tk)
                      {
                            return p.async_write_some(
                                    asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())),
                                    std::move(tk));
                      },
                      py::arg("buffer"));


    register_async_op(cl, "async_write_some",
                      register_tag<pt&, py::buffer>{},
                      [](pt& p, py::buffer b, auto tk)
                      {
                          auto bi = b.request();
                          return p.async_write_some(
                                  asio::buffer(bi.ptr, bi.size),
                                  std::move(tk));
                      },
                      py::arg("buffer"));

    register_async_op(cl, "async_write_some",
                      register_tag<pt&, py::bytes>{},
                      [](pt& p, py::bytes b, auto tk)
                      {

                          char *buffer = nullptr;
                          ssize_t length = 0;
                          PyBytes_AsStringAndSize(b.ptr(), &buffer, &length);
                          return p.async_write_some(
                                  asio::buffer(buffer, length),
                                  std::move(tk));
                      },
                      py::arg("buffer"));
    

    m.def("write", [](pt & p, py::str s)
           {
               return asio::write(p, asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())));
           },
           py::arg("s"),
           py::arg("buffer"));
    m.def("write", [](pt & p, py::buffer b)
    {
        auto bi = b.request();
        return asio::write(p, asio::buffer(bi.ptr, bi.size));
    }, py::arg("s"), py::arg("buffer"));

    m.def("write", [](pt & p, py::bytes b)
    {
        char *buffer = nullptr;
        ssize_t length = 0;
        if (PyBytes_AsStringAndSize(b.ptr(), &buffer, &length))
            return asio::write(p, asio::buffer(buffer, length));
        else
            throw std::runtime_error("Can't read size of buffer");
    }, py::arg("s"), py::arg("buffer"));

    m.def("write", [](pt & p, py::str s, asio::error_code & ec)
    {
        return asio::write(p, asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())), ec);
    }, py::arg("s"), py::arg("buffer"), py::arg("ec"));
    m.def("write", [](pt & p, py::buffer b, asio::error_code & ec)
    {
        auto bi = b.request();
        return asio::write(p, asio::buffer(bi.ptr, bi.size), ec);
    }, py::arg("s"), py::arg("buffer"), py::arg("ec"));


    m.def("write", [](pt & p, py::bytes b, asio::error_code & ec)
    {
        char *buffer = nullptr;
        ssize_t length = 0;
        if (PyBytes_AsStringAndSize(b.ptr(), &buffer, &length))
            return asio::write(p, asio::buffer(buffer, length), ec);
        else
            throw std::runtime_error("Can't read size of buffer");
    }, py::arg("s"), py::arg("buffer"), py::arg("ec"));

    register_async_op(m, "async_write",
                      register_tag<pt&, py::str>{},
                      [](pt& p, py::str s, auto tk)
                      {
                          return asio::async_write(p,
                                  asio::buffer(PyUnicode_1BYTE_DATA(s.ptr()), PyUnicode_GET_LENGTH(s.ptr())),
                                  std::move(tk));
                      },
                      py::arg("s"),
                      py::arg("buffer"));


    register_async_op(m, "async_write_some",
                      register_tag<pt&, py::buffer>{},
                      [](pt& p, py::buffer b, auto tk)
                      {
                          auto bi = b.request();
                          return asio::async_write(p,
                                  asio::buffer(bi.ptr, bi.size),
                                  std::move(tk));
                      },
                      py::arg("s"),
                      py::arg("buffer"));

    register_async_op(m, "async_write",
                      register_tag<pt&, py::bytes>{},
                      [](pt& p, py::bytes b, auto tk)
                      {

                          char *buffer = nullptr;
                          ssize_t length = 0;
                          PyBytes_AsStringAndSize(b.ptr(), &buffer, &length);
                          return asio::async_write(p,
                                  asio::buffer(buffer, length),
                                  std::move(tk));
                      },
                      py::arg("s"),
                      py::arg("buffer"));

}

}

#endif //ASIO_PY_WRITE_HPP
