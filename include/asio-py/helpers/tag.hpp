// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_TAG_HPP
#define ASIO_PY_TAG_HPP

namespace asio_py
{

template<typename ... Args>
struct register_tag {};

namespace detail
{

template<typename Tuple>
struct register_tag_from_tuple;

template<typename ... Args>
struct register_tag_from_tuple<std::tuple<Args...>>
{
    using type = register_tag<Args...>;
};

template<typename Tuple>
using register_tag_from_tuple_t = typename register_tag_from_tuple<Tuple>::type;

}

}

#endif //ASIO_PY_TAG_HPP
