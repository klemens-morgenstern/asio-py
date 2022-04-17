// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_TEMPLATE_HPP
#define ASIO_PY_TEMPLATE_HPP

#include <asio-py/helpers/tag.hpp>

#include <pybind11.h>
//#include <pybind11/.h>
#include <unordered_map>

namespace asio_py
{

namespace detail
{

template<typename ... Args>
struct is_tuple : std::false_type {};

template<typename ... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};


}

template<typename T>
inline pybind11::type type_handle()
{
    namespace py = pybind11;

    if constexpr (std::is_void_v<T>)
        return py::type::of(py::none());
    else if constexpr (std::is_fundamental_v<T>)
        return py::type::of(py::cast(T{}));
    else if constexpr (std::is_same_v<T, std::string>)
        return py::type::of(py::str(""));
    else if constexpr (std::is_same_v<T, py::object>)
        return py::type::of(py::object());
    else if constexpr (std::is_same_v<T, std::tuple<>>)
        return py::type::of(py::none());
    else if constexpr (detail::is_tuple<T>::value)
        return py::type::of(py::tuple());
    else
        return py::type::of<T>();
}


template<template<typename> class Template, typename ... Args>
inline auto make_static_template_picker(register_tag<Args...>)
{
    namespace py = pybind11;

    return [](py::object ob) -> py::object
            {
                const auto  tp = ob.cast<py::type>();
                py::object obj = py::none();
                const auto p =
                    [&]<typename T>(register_tag<T>)
                    {
                        const auto t = type_handle<T>();
                        if (tp.is(t))
                        {
                            obj = type_handle<Template<T>>();
                            return true;
                        }
                        else
                            return false;
                    };

                (p(register_tag<Args>{}) || ... );
                return obj;
            };
}

template<template<typename, typename> class Template, typename ... Args1, typename ... Args2>
inline auto make_static_template_picker(register_tag<Args1...>, register_tag<Args2...>)
{
    namespace py = pybind11;

    return [](py::object ob1, py::object ob2) -> py::object
    {
        const auto tp1 = ob1.cast<py::type>();
        const auto tp2 = ob2.cast<py::type>();
        py::object obj = py::none();
        const auto p =
                [&]<typename T1, typename T2>(register_tag<T1>, register_tag<T2>)
                {
                    const auto t1 = type_handle<T1>();
                    const auto t2 = type_handle<T2>();
                    if (tp1.is(t1) && tp2.is(t2))
                    {
                        obj = type_handle<Template<T1, T2>>();
                        return true;
                    }
                    else
                        return false;
                };

        const auto q =
                [&]<typename T>(register_tag<T> t)
                {
                    return (p(t, register_tag<Args2>{}) || ...);
                };

        (q(register_tag<Args1>{}) || ... );
        return obj;
    };
}


template<template<typename, typename, typename> class Template, typename ... Args1, typename ... Args2, typename ... Args3>
inline auto make_static_template_picker(register_tag<Args1...>, register_tag<Args2...>, register_tag<Args3...>)
{
    namespace py = pybind11;

    return [](py::object ob1, py::object ob2, py::object ob3) -> py::object
    {
        const auto tp1 = ob1.cast<py::type>();
        const auto tp2 = ob2.cast<py::type>();
        const auto tp3 = ob3.cast<py::type>();
        py::object obj = py::none();
        const auto p =
                [&]<typename T1, typename T2, typename T3>(register_tag<T1>, register_tag<T2>, register_tag<T3>)
                {
                    const auto t1 = type_handle<T1>();
                    const auto t2 = type_handle<T2>();
                    const auto t3 = type_handle<T3>();
                    if (tp1.is(t1) && tp2.is(t2) && tp3.is(t3))
                    {
                        obj = type_handle<Template<T1, T2, T3>>();
                        return true;
                    }
                    else
                        return false;
                };

        const auto q =
                [&]<typename T1, typename T2>(register_tag<T1> t1, register_tag<T2> t2)
                {
                    return (p(t1, t2, register_tag<Args3>{}) || ...);
                };

        const auto r =
                [&]<typename T>(register_tag<T> t)
                {
                    return (q(t, register_tag<Args2>{}) || ...);
                };

        (r(register_tag<Args1>{}) || ... );
        return obj;
    };
}

template<template<typename...> class Template, std::size_t Length>
struct dynamic_template_picker_t;

template<template<typename...> class Template>
struct dynamic_template_picker_t<Template, 1u>
{
    std::vector<std::tuple<pybind11::type, pybind11::type>> mp_;
    pybind11::object pick(pybind11::object obj) const
    {
        namespace py = pybind11;

        const auto tp = obj.cast<py::type>();
        const auto l = [&](const auto & p){return get<0>(p).is(tp); };
        if (auto itr = std::find_if(mp_.begin(), mp_.end(), l); itr != mp_.end())
            return get<1>(*itr);
        else
            return py::none();
    }

    template<typename T>
    dynamic_template_picker_t & add_type(register_tag<T> ={})
    {
        namespace py = pybind11;
        mp_.push_back(std::make_tuple(type_handle<T>(), type_handle<Template<T>>()));
        return *this;
    }

    template<typename ... Ts>
    dynamic_template_picker_t & add_types(register_tag<Ts> ... tags)
    {
        namespace py = pybind11;
        (add_type(tags), ...);
        return *this;
    }
};

template<template<typename...> class Template>
struct dynamic_template_picker_t<Template, 2u>
{
    std::vector<std::tuple<pybind11::type, pybind11::type, pybind11::type>> mp_;
    pybind11::object pick(pybind11::tuple tup) const
    {
        namespace py = pybind11;

        const auto tp1 = tup[0].cast<py::type>();
        const auto tp2 = tup[1].cast<py::type>();

        const auto l = [&](const auto & p){return get<0>(p).is(tp1) && get<1>(p).is(tp2); };
        if (auto itr = std::find_if(mp_.begin(), mp_.end(), l); itr != mp_.end())
            return get<2>(*itr);
        else
            return py::none();
    }

    template<typename T, typename U>
    dynamic_template_picker_t & add_type(register_tag<T, U> ={})
    {
        namespace py = pybind11;
        mp_.push_back(std::make_tuple(type_handle<T>(), type_handle<U>(), type_handle<Template<T, U>>()));
        return *this;
    }

    template<typename ... Tags>
    dynamic_template_picker_t & add_types(Tags ... tags)
    {
        namespace py = pybind11;
        (add_type(tags), ...);
        return *this;
    }
};

template<template<typename...> class Template>
struct dynamic_template_picker_t<Template, 3u>
{
    std::vector<std::tuple<pybind11::type, pybind11::type, pybind11::type, pybind11::type>> mp_;
    pybind11::object pick(pybind11::tuple tup) const
    {
        namespace py = pybind11;

        const auto tp1 = tup[0].cast<py::type>();
        const auto tp2 = tup[1].cast<py::type>();
        const auto tp3 = tup[2].cast<py::type>();
        const auto l = [&](const auto & p){return get<0>(p).is(tp1) && get<1>(p).is(tp2) && get<2>(p).is(tp3); };
        if (auto itr = std::find_if(mp_.begin(), mp_.end(), l); itr != mp_.end())
            return get<3>(*itr);
        else
            return py::none();
    }

    template<typename T, typename U, typename V>
    dynamic_template_picker_t & add_type(register_tag<T, U, V> ={})
    {
        namespace py = pybind11;
        mp_.push_back(std::make_tuple(type_handle<T>(), type_handle<U>(), type_handle<V>(), type_handle<Template<T, U , V>>()));
        return *this;
    }

    template<typename ... Tags>
    dynamic_template_picker_t & add_types(Tags ... tags)
    {
        namespace py = pybind11;
        (add_type(tags), ...);
        return *this;
    }
};


template<template<typename> class Tpl>
inline dynamic_template_picker_t<Tpl, 1u> & make_dynamic_template_picker(
        pybind11::module_ & m,
        std::string name,
        std::optional<pybind11::arg_v> arg = std::nullopt)
{
    using type = dynamic_template_picker_t<Tpl, 1u>;
    namespace py = pybind11;
    const auto name_t = "__" + name + "_t";
    py::class_<type> c(m, name_t.c_str());

    if (arg)
        c.def("__call__", &type::pick, *arg);
    else
        c.def("__call__", &type::pick);

    py::object obj = py::cast(type{});

    m.add_object(name.c_str(), obj);
    return py::cast<type&>(obj);
}

template<template<typename, typename> class Tpl>
inline dynamic_template_picker_t<Tpl, 2u> & make_dynamic_template_picker(
        pybind11::module_ & m,
        std::string name,
        std::optional<pybind11::arg_v> arg = std::nullopt)
{
    using type = dynamic_template_picker_t<Tpl, 2u>;
    namespace py = pybind11;
    const auto name_t = "__" + name + "_t";
    py::class_<type> c(m, name_t.c_str());

    if (arg)
        c.def("__call__", &type::pick, *arg);
    else
        c.def("__call__", &type::pick);

    py::object obj = py::cast(type{});

    m.add_object(name.c_str(), obj);
    return py::cast<type&>(obj);
}

template<template<typename, typename, typename> class Tpl>
inline dynamic_template_picker_t<Tpl, 3u> & make_dynamic_template_picker(
        pybind11::module_ & m,
        std::string name,
        std::optional<pybind11::arg_v> arg = std::nullopt)
{
    using type = dynamic_template_picker_t<Tpl, 3u>;
    namespace py = pybind11;
    const auto name_t = "__" + name + "_t";
    py::class_<type> c(m, name_t.c_str());

    if (arg)
        c.def("__call__", &type::pick, *arg);
    else
        c.def("__call__", &type::pick);

    py::object obj = py::cast(type{});

    m.add_object(name.c_str(), obj);
    return py::cast<type&>(obj);
}


}

#endif //ASIO_PY_TEMPLATE_HPP
