// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_AWAITABLE_HPP
#define ASIO_PY_AWAITABLE_HPP

#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio-py/detail/exceptions.hpp>
#include <asio-py/helpers/template.hpp>
#include <asio-py/detail/typename.hpp>
#include <asio-py/helpers/options.hpp>
#include <pybind11.h>
#include <pybind11/eval.h>

namespace asio_py
{

struct awaitable_wrapper
{
    struct base
    {
        virtual bool valid() const = 0;
        virtual pybind11::type value_type() const = 0;
        virtual pybind11::type executor_type() const = 0;
        virtual asio::awaitable<pybind11::object> await()  && = 0;
        virtual ~base() = default;
    };

    template<typename T, typename Executor>
    struct impl final : base
    {
        asio::awaitable<T, Executor> aw;
        impl(asio::awaitable<T, Executor> aw) : aw(std::move(aw)) {}

        bool valid() const override
        {
            return aw.valid();
        }
        pybind11::type value_type()  const override
        {
            return type_handle<T>();
        }
        pybind11::type executor_type() const override
        {
            return pybind11::type::of<Executor>();
        }

        asio::awaitable<pybind11::object> await()  && override
        {
            if constexpr (std::is_void_v<T>)
            {
                if constexpr (std::is_same_v<Executor, asio::any_io_executor>)
                    co_await std::move(aw);
                else
                    co_await asio::co_spawn(co_await asio::this_coro::executor, std::move(aw), asio::use_awaitable);

                co_return pybind11::none();
            }
            else if constexpr (std::is_same_v<T, pybind11::object>)
            {
                if constexpr (std::is_same_v<Executor, asio::any_io_executor>)
                    co_return co_await std::move(aw);
                else
                    co_return co_await asio::co_spawn(co_await asio::this_coro::executor, std::move(aw), asio::use_awaitable);
            }
            else
            {
                if constexpr (std::is_same_v<Executor, asio::any_io_executor>)
                    co_return pybind11::cast(co_await std::move(aw));
                else
                    co_return pybind11::cast(co_await asio::co_spawn(co_await asio::this_coro::executor, std::move(aw), asio::use_awaitable));
            }
        }
    };


    bool valid() const
    {
        return ptr && ptr->valid();
    }

    pybind11::type value_type() const
    {
        if (!ptr)
            throw std::runtime_error("awaitable invalid");
        return ptr->value_type();
    }
    pybind11::type executor_type() const
    {
        if (!ptr)
            throw std::runtime_error("awaitable invalid");
        return ptr->executor_type();
    }
    asio::awaitable<pybind11::object> await() &&
    {
        if (!valid())
            throw std::runtime_error("awaitable expired");
        return std::move(*ptr).await();
    };

    std::unique_ptr<base> ptr;

    awaitable_wrapper() = delete;

    template<typename T, typename Executor>
    awaitable_wrapper(asio::awaitable<T, Executor> aw)
        : ptr(std::make_unique<impl<T, Executor>>(std::move(aw))) {}

    template<typename T, typename Executor>
    asio::awaitable<T, Executor> cast_and_steal()
    {
        auto p = dynamic_cast<impl<T, Executor>*>(ptr.get());
        if (p)
            return std::move(p->aw);
        return {};
    };

    awaitable_wrapper(const awaitable_wrapper & ) = delete;
    awaitable_wrapper(awaitable_wrapper &&) = default;

    ~awaitable_wrapper()
    {
    }
};




inline void register_awaitable(pybind11::module_ & m)
{
    namespace py = pybind11;
    py::class_<awaitable_wrapper> res(m, "awaitable");

    res.def("valid", &awaitable_wrapper::valid);
    res.def_property_readonly("value_type",     &awaitable_wrapper::value_type);
    res.def_property_readonly("executor_type",  &awaitable_wrapper::executor_type);

    py::dict locals = py::dict();
    py::exec(
    R"(
def __await_impl(self):
    res = yield self
    return res)", py::globals(), locals);
    py::function await_impl = locals["__await_impl"];
    setattr(res, "__await__", await_impl);

/*
    py::class_<asio::awaitable<py::object>> res(m, "awaitable");
    res.def("valid", &asio::awaitable<py::object>::valid);
    res.attr("executor_type") = type_handle<asio::any_io_executor>();
    res.attr("value_type") = type_handle<void>();


    py::dict locals = py::dict();
    py::exec(
R"(def __await_impl(self):
    res = yield self.__to_awaitable_wrapper()
    return res)", py::globals(), locals);
    py::function await_impl = locals["__await_impl"];
    setattr(res, "__await__", await_impl);*/
}

template<typename T, typename Executor>
inline void register_awaitable(
        pybind11::module_ & m,
        std::string name = "awaitable_" + detail::get_sanitized_type_name<T>() + "_" + detail::get_sanitized_type_name<Executor>())
{
    if (hasattr(m, name.c_str()))
        return ;

    namespace py = pybind11;

    const auto nm = std::is_same_v<Executor, asio::any_io_executor> ? "awaitable" : "awaitable_" + detail::get_sanitized_type_name<Executor>();

    if (!hasattr(m, nm.c_str()))
    {
        py::class_<asio::awaitable<py::object, Executor>> cc(m, nm.c_str());
        cc.def("valid", &asio::awaitable<py::object, Executor>::valid);
        cc.attr("executor_type") = type_handle<Executor>();
    }

    py::dict locals = py::dict();
    py::exec(R"(def __await_impl(self):
    res = yield self.__to_awaitable_wrapper()
    return res)", py::globals(), locals);
    py::function await_impl = locals["__await_impl"];

    using aw_t = asio::awaitable<T, Executor>;
    py::class_<aw_t> cc(m, name.c_str());

    cc.def("__to_awaitable_wrapper",
           [](aw_t & p) -> asio::awaitable<pybind11::object, Executor>
           {
                if constexpr (std::is_void_v<T>)
                {
                    co_await std::move(p);
                    co_return py::none();
                }
                else
                {
                    auto v = co_await std::move(p);
                    co_return py::cast(v);
                }
           })
       .def("valid", &aw_t::valid);

    cc.attr("executor_type") = type_handle<Executor>();
    cc.attr("value_type")    = type_handle<T>();

    setattr(cc, "__await__", await_impl);
}

template<typename T>
inline void register_awaitable(pybind11::module_ & m)
{
    [&]<typename ... Executors>(register_tag<Executors...>)
        {
            (register_awaitable<T, Executors>(m), ...);
        }(all_executors{});
}

}


namespace pybind11::detail
{

template < typename T, typename Executor >
struct type_caster< asio::awaitable<T, Executor> >
{
  public:
    using value_conv = make_caster<T>;
    using executor_conv = make_caster<Executor>;

    using aw_t = asio::awaitable<T, Executor>;

    PYBIND11_TYPE_CASTER(aw_t, const_name("awaitable[") + value_conv::name + const_name(", ") + executor_conv::name + const_name("]"));

    bool
    load(const handle & h, bool b)
    {
        if (isinstance<asio_py::awaitable_wrapper>(h))
        {
            auto &ww = pybind11::cast<asio_py::awaitable_wrapper&>(h);
            auto aw = ww.cast_and_steal<T, Executor>();
            if (aw.valid())
            {
                value = std::move(aw);
                return true;
            }
        }
        return false;
    }

    static handle
    cast(asio::awaitable<T, Executor> aw, return_value_policy /* policy */, handle /* parent */)
    {
        return pybind11::cast(asio_py::awaitable_wrapper(std::move(aw))).release();
    }
};

}


#endif //ASIO_PY_AWAITABLE_HPP
