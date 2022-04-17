// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_HELPER_HPP
#define ASIO_PY_HELPER_HPP

#include <asio-py/helpers/tag.hpp>

namespace asio_py
{

namespace detail
{


template<typename Tup, typename T>
struct insert_set
{
};

template<typename ... Args, typename T>
struct insert_set<register_tag<Args...>, T>
{
    using type = std::conditional_t<(std::is_same_v<Args, T> || ...), register_tag<Args...>, register_tag<Args..., T>>;
};

template<typename Tup, typename T>
using insert_set_t = typename insert_set<Tup, T>::type;


template<typename Tuple, typename Next, typename ...Rest>
struct as_set_impl
{
    using type = typename as_set_impl<insert_set_t<Tuple, Next>, Rest...>::type;
};

template<typename Tuple, typename Next>
struct as_set_impl<Tuple, Next>
{
    using type = insert_set_t<Tuple, Next>;
};

template<typename Tuple>
struct as_set;

template<>
struct as_set<register_tag<>> { using type = register_tag<>; };

template<typename T>
struct as_set<register_tag<T>> { using type = register_tag<T>; };

template<typename ... Args>
struct as_set<register_tag<Args...>>
{
    using type = typename as_set_impl<register_tag<>, Args...>::type;
};

template<typename Tuple>
using as_set_t = typename as_set<Tuple>::type;

template<typename ... Args>
struct tuple_cat;

template<typename ...Ts>
struct tuple_cat<register_tag<Ts...>>
{
    using type = register_tag<Ts...>;
};

template<>
struct tuple_cat<>
{
    using type = register_tag<>;
};

template<typename ...Ts, typename ... Us>
struct tuple_cat<register_tag<Ts...>, register_tag<Us...>>
{
    using type = register_tag<Ts..., Us...>;
};

template<typename ...Ts, typename ... Us, typename ... Vs>
struct tuple_cat<register_tag<Ts...>, register_tag<Us...>, register_tag<Vs...>>
{
    using type = register_tag<Ts..., Us..., Vs...>;
};

template<typename ...Ts, typename ... Us, typename ... Vs, typename ... Ws>
struct tuple_cat<register_tag<Ts...>, register_tag<Us...>, register_tag<Vs...>, register_tag<Ws...>>
{
    using type = register_tag<Ts..., Us..., Vs..., Ws...>;
};

template<typename Ts, typename ... Rest>
struct tuple_cat<Ts, Rest...>
{
    using type = typename tuple_cat<Ts, typename tuple_cat<Rest...>::type>::type;
};

template<typename ... Args>
using tuple_cat_t = typename tuple_cat<Args...>::type;


template<typename Elem, typename Tup>
struct push_front;

template<typename Elem, typename ... Ts>
struct push_front<Elem, register_tag<Ts...>>
{
    using type = register_tag<Elem, Ts...>;
};

template<typename Elem, typename Tup>
struct remove;

template<typename Elem, typename First, typename ... Ts>
struct remove<Elem, register_tag<First, Ts...>>
{
    using next = typename remove<Elem, register_tag<Ts...>>::type;
    using type = std::conditional_t<std::is_same_v<Elem, First>,
                                    next,
                                    typename push_front<First, next>::type>;
};

template<typename Elem>
struct remove<Elem, register_tag<>>
{
    using type = register_tag<>;
};


}

}

#endif //ASIO_PY_HELPER_HPP
