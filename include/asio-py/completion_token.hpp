// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_COMPLETION_TOKEN_HPP
#define ASIO_PY_COMPLETION_TOKEN_HPP

#include <pybind11/stl.h>
#include <asio/detached.hpp>
#include <asio/experimental/as_tuple.hpp>
#include <asio/bind_cancellation_slot.hpp>
#include <asio/bind_executor.hpp>
#include <asio/use_awaitable.hpp>
#include <asio-py/helpers/tag.hpp>
#include <asio-py/awaitable.hpp>

namespace asio_py
{

namespace detail
{

struct wrap_cb
{
    pybind11::function func;

    template<typename ... Args>
    void operator()(Args && ... args) const
    {
        pybind11::print("~123 0");
        pybind11::print("~123 ", get_type_name<Args>()...);
        func(std::forward<Args>(args)...);
    }

    void operator()(std::exception_ptr e, pybind11::object obj) const
    {

        namespace py = pybind11;
        if (!obj)
            obj = py::none();

        if (obj.is_none())
            try
            {
                func(e, obj);
            }
            catch(py::type_error & te)
            {
                std::string_view msg{te.what()};
                if (msg.find("takes 1 positional argument but 2 were given") != std::string_view::npos)
                    func(e);
                else
                    throw;
            }
            catch (py::error_already_set & es)
            {
                if (py::cast<std::string>(py::str(es.type())) != "<class 'TypeError'>")
                    throw;
                else
                    if (std::string_view{es.what()}.find("takes 1 positional argument but 2 were given") != std::string_view::npos)
                        func(e);
                    else
                        throw;
            }
        else
            func(py::cast(e), obj);

    }

};

}

template<typename Mod, typename ... Args, typename ...Argv, typename FuncPicker>
void register_async_op(Mod & m, const char * name,
                       register_tag<Args...>,
                       FuncPicker && fp,
                       Argv && ... argv)
{
    namespace py = pybind11;

    m.def(name,
        [fp](
                Args ... args,
                py::function cb_,
                std::optional<asio::cancellation_slot> cl,
                std::optional<asio::any_io_executor> exec,
                bool as_tuple)
        {
            auto cb = detail::wrap_cb{std::move(cb_)};

            if ((!!cl) && exec && as_tuple)
                fp(std::forward<Args>(args)...,
                              asio::bind_cancellation_slot(*cl,
                                     asio::bind_executor(*exec,
                                     asio::experimental::as_tuple(cb))));
            else if (exec && as_tuple)
                fp(std::forward<Args>(args)...,
                   asio::bind_executor(*exec, asio::experimental::as_tuple(cb)));
            else if ((!!cl) && as_tuple)
                fp(std::forward<Args>(args)...,
                   asio::bind_cancellation_slot(*cl, asio::experimental::as_tuple(cb)));
            else if ((!!cl) && exec)
                fp(std::forward<Args>(args)...,
                   asio::bind_cancellation_slot(*cl, asio::bind_executor(*exec, cb)));
            else if (!!cl)
                fp(std::forward<Args>(args)..., asio::bind_cancellation_slot(*cl, cb));
            else if (exec)
                fp(std::forward<Args>(args)..., asio::bind_executor(*exec, cb));
            else if (as_tuple)
                fp(std::forward<Args>(args)..., asio::experimental::as_tuple(cb));
            else
                fp(std::forward<Args>(args)..., cb);

        },
        std::forward<Argv>(argv)...,
        py::arg("callback"),
        py::arg("cancellation_slot") = std::nullopt,
        py::arg("executor") = std::nullopt,
        py::arg("as_tuple") = false);

    m.def(name,
          [fp](
                  Args ... args,
                  asio::detached_t cb,
                  std::optional<asio::cancellation_slot> cl,
                  std::optional<asio::any_io_executor> exec,
                  bool as_tuple)
          {
              if ((!!cl) && exec && as_tuple)
                  fp(std::forward<Args>(args)...,
                     asio::bind_cancellation_slot(*cl,
                                                  asio::bind_executor(*exec,
                                                                      asio::experimental::as_tuple(cb))));
              else if (exec && as_tuple)
                  fp(std::forward<Args>(args)...,
                     asio::bind_executor(*exec, asio::experimental::as_tuple(cb)));
              else if ((!!cl) && as_tuple)
                  fp(std::forward<Args>(args)...,
                     asio::bind_cancellation_slot(*cl, asio::experimental::as_tuple(cb)));
              else if ((!!cl) && exec)
                  fp(std::forward<Args>(args)...,
                     asio::bind_cancellation_slot(*cl, asio::bind_executor(*exec, cb)));
              else if (!!cl)
                  fp(std::forward<Args>(args)..., asio::bind_cancellation_slot(*cl, cb));
              else if (exec)
                  fp(std::forward<Args>(args)..., asio::bind_executor(*exec, cb));
              else if (as_tuple)
                  fp(std::forward<Args>(args)..., asio::experimental::as_tuple(cb));
              else
                  fp(std::forward<Args>(args)..., cb);

          },
          std::forward<Argv>(argv)...,
          py::arg("token"),
          py::arg("cancellation_slot") = std::nullopt,
          py::arg("executor") = std::nullopt,
          py::arg("as_tuple") = false);

    m.def(name,
        [fp_ = fp](
                Args ... args,
                std::optional<asio::cancellation_slot> cl,
                std::optional<asio::any_io_executor> exec,
                bool as_tuple)
        {
            auto fp =
                    [fp_]<typename ... Args_>(Args_ && ... args) mutable
                    {
                        return awaitable_wrapper(std::move(fp_)(std::forward<Args_>(args)...));
                    };
            if ((!!cl) && exec && as_tuple)
                return fp(std::forward<Args>(args)...,
                   asio::bind_cancellation_slot(*cl,
                                                asio::bind_executor(*exec,
                                                    asio::experimental::as_tuple(asio::use_awaitable))));
            else if (exec && as_tuple)
                return fp(std::forward<Args>(args)...,
                   asio::bind_executor(*exec, asio::experimental::as_tuple(asio::use_awaitable)));
            else if ((!!cl) && as_tuple)
                return fp(std::forward<Args>(args)...,
                   asio::bind_cancellation_slot(*cl, asio::experimental::as_tuple(asio::use_awaitable)));
            else if ((!!cl) && exec)
                return fp(std::forward<Args>(args)...,
                   asio::bind_cancellation_slot(*cl, asio::bind_executor(*exec, asio::use_awaitable)));
            else if (!!cl)
                return fp(std::forward<Args>(args)..., asio::bind_cancellation_slot(*cl, asio::use_awaitable));
            else if (exec)
                return fp(std::forward<Args>(args)..., asio::bind_executor(*exec, asio::use_awaitable));
            else if (as_tuple)
                return fp(std::forward<Args>(args)..., asio::experimental::as_tuple(asio::use_awaitable));
            else
                return fp(std::forward<Args>(args)..., asio::use_awaitable);
       },
        std::forward<Argv>(argv)...,
        py::arg("cancellation_slot") = std::nullopt,
        py::arg("executor") = std::nullopt,
        py::arg("as_tuple") = false);
}

}

#endif //ASIO_PY_COMPLETION_TOKEN_HPP
