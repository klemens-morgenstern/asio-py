// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_READ_HPP
#define ASIO_PY_READ_HPP

#include <pybind11.h>

#include <asio/read.hpp>
#include <asio/read_until.hpp>

#include <asio-py/helpers/tag.hpp>

namespace asio_py
{

template<typename Cl>
void register_read(pybind11::module_ & m, pybind11::class_<Cl> & cl)
{
    using pt = Cl;
    namespace py = pybind11;


    cl.def( "read_some",
            [](pt & p, py::object b, std::size_t sz) -> py::object
            {
                const auto tt = py::type(b);
                if (tt.is(py::type::of(py::str())))
                {
                    // str
                    std::string buf(' ', sz);
                    const auto sz = p.read_some(asio::buffer(buf));
                    return py::str(buf.c_str(), sz);
                }
                else if (tt.is(py::type::of(py::bytes())))
                {
                    std::vector<char> buf(sz);
                    const auto sz = p.read_some(asio::buffer(buf));
                    return py::bytes(buf.data(), sz);
                }
                throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
            },
            py::arg("type") = py::type::of(py::bytes()),
            py::arg("size") = PIPE_BUF);

    cl.def( "read_some",
            [](pt & p, asio::error_code & ec, py::object b, std::size_t sz) -> py::object
            {
                const auto tt = py::type(b);
                if (tt.is(py::type::of(py::str())))
                {
                    std::string buf(' ', sz);
                    const auto sz = p.read_some(asio::buffer(buf), ec);
                    return py::str(buf.c_str(), sz);
                }
                else if (tt.is(py::type::of(py::bytes())))
                {
                    std::vector<char> buf(sz);
                    const auto sz = p.read_some(asio::buffer(buf), ec);
                    return py::bytes(buf.data(), sz);
                }
                throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
            },
            py::arg("ec"),
            py::arg("type") = py::type::of(py::bytes()),
            py::arg("size") = PIPE_BUF);

    register_async_op(cl, "async_read_some",
                      register_tag<pt&, py::object, std::size_t>{},
                      [](pt& p, py::object b, std::size_t sz, auto tk)
                      {

                          const auto tt = py::type(b);
                          if (tt.is(py::type::of(py::str())))
                          {
                              // str
                              auto buf = std::make_shared<std::string>(' ', sz);
                              return p.async_read_some(asio::buffer(*buf), asio::experimental::deferred
                                      ([buf](asio::error_code ec, std::size_t sz)
                                               -> asio::experimental::deferred_values<asio::error_code, py::object>
                                       {
                                           return asio::experimental::deferred.values(
                                                   ec,
                                                   py::object(py::str(buf->c_str(), sz)));
                                       }))(std::move(tk));

                          }
                          else if (tt.is(py::type::of(py::bytes())))
                          {
                              auto buf = std::make_shared<std::vector<char>>(sz);
                              return p.async_read_some(asio::buffer(*buf), asio::experimental::deferred
                                      ([buf](asio::error_code ec, std::size_t sz) mutable
                                               -> asio::experimental::deferred_values<asio::error_code, py::object>
                                       {
                                           return asio::experimental::deferred.values(
                                                   ec,
                                                   py::object(py::bytes(buf->data(), sz)));
                                       }))(std::move(tk));

                          }

                          return asio::post(p.get_executor(), asio::experimental::deferred(
                                  []() mutable -> asio::experimental::deferred_values<asio::error_code, py::object>
                                  {
                                      return asio::experimental::deferred.values(
                                              asio::error_code{asio::error::invalid_argument},
                                              py::object(py::none()));
                                  }))(std::move(tk));
                      },
                      py::arg("type") = py::type::of(py::bytes()),
                      py::arg("size") = PIPE_BUF);

    m.def( "read",
            [](pt & p, py::object b, std::size_t sz) -> py::object
            {
                const auto tt = py::type(b);
                if (tt.is(py::type::of(py::str())))
                {
                    // str
                    std::string buf(' ', sz);
                    const auto sz = asio::read(p, asio::buffer(buf));
                    return py::str(buf.c_str(), sz);
                }
                else if (tt.is(py::type::of(py::bytes())))
                {
                    std::vector<char> buf(sz);
                    const auto sz = asio::read(p, asio::buffer(buf));
                    return py::bytes(buf.data(), sz);
                }
                throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
            },
            py::arg("s"),
            py::arg("type") = py::type::of(py::bytes()),
            py::arg("size") = PIPE_BUF);

    m.def( "read",
            [](pt & p, asio::error_code & ec, py::object b, std::size_t sz) -> py::object
            {
                const auto tt = py::type(b);
                if (tt.is(py::type::of(py::str())))
                {
                    std::string buf(' ', sz);
                    const auto sz = asio::read(p, asio::buffer(buf), ec);
                    return py::str(buf.c_str(), sz);
                }
                else if (tt.is(py::type::of(py::bytes())))
                {
                    std::vector<char> buf(sz);
                    const auto sz = asio::read(p, asio::buffer(buf), ec);
                    return py::bytes(buf.data(), sz);
                }
                throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
            },
            py::arg("s"),
            py::arg("ec"),
            py::arg("type") = py::type::of(py::bytes()),
            py::arg("size") = PIPE_BUF);

    register_async_op(m, "async_read",
          register_tag<pt&, py::object, std::size_t>{},
          [](pt& p, py::object b, std::size_t sz, auto tk)
          {
               const auto tt = py::type(b);
               if (tt.is(py::type::of(py::str())))
               {
                   // str
                   auto buf = std::make_shared<std::string>(' ', sz);
                   return asio::async_read(p, asio::buffer(*buf), asio::experimental::deferred
                           ([buf](asio::error_code ec, std::size_t sz)
                                    -> asio::experimental::deferred_values<asio::error_code, py::object>
                            {
                                return asio::experimental::deferred.values(
                                        ec,
                                        py::object(py::str(buf->c_str(), sz)));
                            }))(std::move(tk));

               }
               else if (tt.is(py::type::of(py::bytes())))
               {
                   auto buf = std::make_shared<std::vector<char>>(sz);
                   return asio::async_read(p, asio::buffer(*buf), asio::experimental::deferred
                           ([buf](asio::error_code ec, std::size_t sz) mutable
                                    -> asio::experimental::deferred_values<asio::error_code, py::object>
                            {
                                return asio::experimental::deferred.values(
                                        ec,
                                        py::object(py::bytes(buf->data(), sz)));
                            }))(std::move(tk));

               }

               return asio::post(p.get_executor(), asio::experimental::deferred(
                       []() mutable -> asio::experimental::deferred_values<asio::error_code, py::object>
                       {
                           return asio::experimental::deferred.values(
                                   asio::error_code{asio::error::invalid_argument},
                                   py::object(py::none()));
                       }))(std::move(tk));
           },
           py::arg("s"),
           py::arg("type") = py::type::of(py::bytes()),
           py::arg("size") = PIPE_BUF);

    m.def( "read_until",
           [](pt & p, asio::string_view delim, py::object b) -> py::object
           {
               const auto tt = py::type(b);
               if (tt.is(py::type::of(py::str())))
               {
                   // str
                   std::string buf;
                   const auto sz = asio::read_until(p, asio::dynamic_buffer(buf), delim);
                   return py::str(buf.c_str(), sz);
               }
               else if (tt.is(py::type::of(py::bytes())))
               {
                   std::vector<char> buf;
                   const auto sz = asio::read_until(p, asio::dynamic_buffer(buf), delim);
                   return py::bytes(buf.data(), sz);
               }
               throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
           },
           py::arg("s"),
           py::arg("delim"),
           py::arg("type") = py::type::of(py::bytes()));

    m.def( "read_until",
           [](pt & p, asio::string_view delim, asio::error_code & ec, py::object b) -> py::object
           {
               const auto tt = py::type(b);
               if (tt.is(py::type::of(py::str())))
               {
                   std::string buf;
                   const auto sz = asio::read_until(p, asio::dynamic_buffer(buf), delim, ec);
                   return py::str(buf.c_str(), sz);
               }
               else if (tt.is(py::type::of(py::bytes())))
               {
                   std::vector<char> buf;
                   const auto sz = asio::read_until(p, asio::dynamic_buffer(buf), delim, ec);
                   return py::bytes(buf.data(), sz);
               }
               throw py::type_error("Invalid buffer type " + py::cast<std::string>(py::str(tt)));
           },
           py::arg("s"),
           py::arg("delim"),
           py::arg("ec"),
           py::arg("type") = py::type::of(py::bytes()));

    register_async_op(m, "async_read_until",
                      register_tag<pt&, asio::string_view, py::object>{},
                      [](pt& p, asio::string_view delim, py::object b, auto tk)
                      {
                          const auto tt = py::type(b);
                          if (tt.is(py::type::of(py::str())))
                          {
                              // str
                              auto buf = std::make_shared<std::string>();
                              return asio::async_read_until(p, asio::dynamic_buffer(*buf), delim, asio::experimental::deferred
                                      ([buf](asio::error_code ec, std::size_t sz)
                                               -> asio::experimental::deferred_values<asio::error_code, py::object>
                                       {
                                           return asio::experimental::deferred.values(
                                                   ec,
                                                   py::object(py::str(buf->c_str(), sz)));
                                       }))(std::move(tk));

                          }
                          else if (tt.is(py::type::of(py::bytes())))
                          {
                              auto buf = std::make_shared<std::vector<char>>();
                              return asio::async_read_until(p, asio::dynamic_buffer(*buf), delim, asio::experimental::deferred
                                      ([buf](asio::error_code ec, std::size_t sz) mutable
                                               -> asio::experimental::deferred_values<asio::error_code, py::object>
                                       {
                                           return asio::experimental::deferred.values(
                                                   ec,
                                                   py::object(py::bytes(buf->data(), sz)));
                                       }))(std::move(tk));

                          }

                          return asio::post(p.get_executor(), asio::experimental::deferred(
                                  []() mutable -> asio::experimental::deferred_values<asio::error_code, py::object>
                                  {
                                      return asio::experimental::deferred.values(
                                              asio::error_code{asio::error::invalid_argument},
                                              py::object(py::none()));
                                  }))(std::move(tk));
                      },
                      py::arg("s"),
                      py::arg("delim"),
                      py::arg("type") = py::type::of(py::bytes()));
}

}

#endif //ASIO_PY_READ_HPP
