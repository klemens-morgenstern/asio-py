// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_OPTIONS_HPP
#define ASIO_PY_OPTIONS_HPP

#include <tuple>

#include <asio/io_context.hpp>
#include <asio/thread_pool.hpp>
#include <asio/system_context.hpp>
#include <asio/redirect_error.hpp>
#include <asio/bind_cancellation_slot.hpp>
#include <asio/bind_executor.hpp>
#include <asio/experimental/as_single.hpp>
#include <asio/experimental/as_tuple.hpp>
#include <asio/use_future.hpp>
#include <asio/use_awaitable.hpp>

#include <asio-py/detail/helper.hpp>

#include <pybind11/functional.h>
#include <asio/strand.hpp>

namespace asio_py
{

struct _ {};

using contexts  = register_tag<asio::io_context, asio::thread_pool, asio::system_context>;
using executors = register_tag<asio::any_io_executor>;

using completion_tokens = register_tag<
                                    asio::use_awaitable_t<>,
                                    asio::use_future_t<>,
                                    pybind11::function>;
using token_modifiers = register_tag<asio::executor_binder<_, _>,
                                     asio::experimental::as_tuple_t<_>,
                                     asio::cancellation_slot_binder<_, _>>;


using cancellation_slots = register_tag<asio::cancellation_slot>;

using executor_properties = register_tag<
        asio::execution::blocking_t,
        asio::execution::blocking_t::always_t,
        asio::execution::blocking_t::never_t,
        asio::execution::blocking_t::possibly_t,
        asio::execution::outstanding_work_t,
        asio::execution::outstanding_work_t::tracked_t,
        asio::execution::outstanding_work_t::untracked_t,
        asio::execution::relationship_t,
        asio::execution::relationship_t::fork_t,
        asio::execution::relationship_t::continuation_t>;

using default_completion_token = asio::use_awaitable_t<>;




namespace detail
{

template<typename Executor, typename Property>
struct apply_properties;


template<typename Executor>
struct apply_properties<Executor, register_tag<>>
{
    using type = register_tag<>;
};

template<typename Executor, typename First, typename ... Properties>
struct apply_properties<Executor, register_tag<First, Properties...>>
{
    constexpr static bool can_require = asio::can_require<Executor, First>::value;

    using mine = std::conditional_t<can_require, std::decay_t<typename asio::require_result<Executor, First>::type>, Executor>;
    constexpr static bool is_modified = !std::is_same_v<mine, Executor>;

    using other_type = std::conditional_t<is_modified, register_tag<mine>, register_tag<>>;
    using cascade = std::conditional_t<is_modified, typename apply_properties<mine, register_tag<Properties...>>::type, register_tag<>>;

    using next = typename apply_properties<Executor, register_tag<Properties...>>::type;
    using type = as_set_t<tuple_cat_t<other_type, cascade, next>>;
};

template<typename Executor, typename Property>
using apply_properties_t = typename apply_properties<Executor, Property>::type;

template<typename Executor, typename  Properties>
struct apply_properties_multiple;

template<typename ... Execs, typename  Properties>
struct apply_properties_multiple<register_tag<Execs...>, Properties>
{
    using type = tuple_cat_t<apply_properties_t<Execs, Properties>...>;
};

template<typename Executor, typename  Properties>
using apply_properties_multiple_t = typename apply_properties_multiple<Executor, Properties>::type;


template<typename Contexts>
struct context_executors;

template<typename ...Contexts>
struct context_executors<register_tag<Contexts...>>
{
    using type = register_tag<typename Contexts::executor_type...>;
};

template<typename Contexts>
using context_executors_t = typename context_executors<Contexts>::type;

template<typename Contexts, typename Executors>
using all_base_executors_t = tuple_cat_t<context_executors_t<Contexts>, Executors>;

template<typename Tup>
struct add_strand;

template<typename ... Args>
struct add_strand<register_tag<Args...>>
{
    using type = register_tag<asio::strand<Args>...>;
};

template<typename Tup>
using add_strand_t = typename add_strand<Tup>::type;

template< typename Properties, typename Executors>
struct all_executors;

template<typename Properties, typename ... Executors>
struct all_executors<Properties, register_tag<Executors...>>
{
    using type = tuple_cat_t<register_tag<Executors...>,
                             apply_properties_t<Executors, Properties> ...>;
};

}


using all_base_executors = detail::all_base_executors_t<contexts, executors>;
using all_executors = detail::as_set_t<
        detail::tuple_cat_t<
                all_base_executors,
                detail::apply_properties_multiple_t<all_base_executors, executor_properties>
        >>;

using all_executors_with_strands = detail::as_set_t<
        detail::tuple_cat_t<
                all_base_executors,
                detail::apply_properties_multiple_t<all_base_executors, executor_properties>
        >>;


// all completion tokens with modifiers.
namespace detail
{

template<typename Token, typename Modifiers, typename Executors, typename CancellationSlotss>
struct apply_modifiers;

template<typename Token, typename ModifierToApply, typename AllModifiers, typename Executors,  typename CancellationSlots>
struct apply_modifier_step;

template<typename Token, template<typename> class Modifier, typename ...AllModifiers, typename Executors,  typename CancellationSlots>
struct apply_modifier_step<Token, Modifier<_>, register_tag<AllModifiers...>, Executors, CancellationSlots>
{
    using modifier_rest = typename detail::remove<Modifier<_>, register_tag<AllModifiers...>>::type;
    using applied = Modifier<Token>;
    //apply all the remaining modifiers
    using rest = typename apply_modifiers<applied, modifier_rest, Executors,  CancellationSlots>::type;
    using type = typename push_front<applied, rest>::type;
};

template<typename Token, typename ...AllModifiers, typename  Executors,  typename ... CancellationSlots>
struct apply_modifier_step<Token, asio::cancellation_slot_binder<_, _>, register_tag<AllModifiers...>, Executors, register_tag<CancellationSlots...>>
{
    using modifier_rest = typename detail::remove<asio::cancellation_slot_binder<_, _>, register_tag<AllModifiers...>>::type;
    using applied = register_tag<asio::cancellation_slot_binder<Token, CancellationSlots>...>;
    //apply all the remaining modifiers
    using rest = typename tuple_cat< typename apply_modifiers<asio::cancellation_slot_binder<Token, CancellationSlots>, modifier_rest, Executors,  register_tag<CancellationSlots ...>>::type...>::type;
    using type = typename tuple_cat< applied, rest >::type;
};

template<typename Token, typename ...AllModifiers, typename ... Executors,  typename CancellationSlots>
struct apply_modifier_step<Token, asio::executor_binder<_, _>, register_tag<AllModifiers...>, register_tag<Executors...>, CancellationSlots>
{
    using modifier_rest = typename detail::remove<asio::executor_binder<_, _>, register_tag<AllModifiers...>>::type;
    using applied = register_tag<asio::executor_binder<Token, Executors>...>;
    //apply all the remaining modifiers
    using rest = typename tuple_cat< typename apply_modifiers<asio::executor_binder<Token, Executors>, modifier_rest, register_tag<Executors...>,  CancellationSlots>::type...>::type;
    using type = typename tuple_cat< applied, rest >::type;
};

template<typename Token, typename ...Modifiers, typename Executors, typename CancellationSlots>
struct apply_modifiers<Token, register_tag<Modifiers...>, Executors, CancellationSlots>
{
    using type = typename tuple_cat<
            typename apply_modifier_step<Token, Modifiers, register_tag<Modifiers...>, Executors, CancellationSlots>::type...>::type;
};

template<typename CompletionToken, typename Modifiers, typename Executors, typename CancellationSlots>
struct modify_completion_token;

template<typename CompletionTokens, typename Modifiers, typename Executors, typename CancellationSlots>
struct all_completion_tokens;

template<typename ... CompletionTokens, typename Modifiers, typename Executors, typename CancellationSlots>
struct all_completion_tokens<register_tag<CompletionTokens...>, Modifiers, Executors, CancellationSlots>
{
    using type = typename tuple_cat<typename apply_modifiers<CompletionTokens, Modifiers, Executors, CancellationSlots>::type...>::type;
};

}
using all_completion_tokens = typename detail::all_completion_tokens<completion_tokens, token_modifiers, all_base_executors, cancellation_slots>::type;

}

#endif //ASIO_PY_OPTIONS_HPP
