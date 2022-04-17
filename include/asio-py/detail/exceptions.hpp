// Copyright (c) 2021 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASIO_PY_EXCEPTIONS_HPP
#define ASIO_PY_EXCEPTIONS_HPP

#include <exception>
#include <chrono>
#include <pybind11.h>

namespace asio_py::detail
{
pybind11::object
inline translate_exception(std::exception_ptr ex)
{
    namespace py = pybind11;
    if (!ex)
        return py::none();
    bool set = false;
    try
    {
        std::rethrow_exception(ex);
    }
    catch(pybind11::error_already_set & eas)
    {
        return eas.value();
    }
    catch(...)
    {
    }

    if (!set)
    {
        auto &registered_exception_translators = pybind11::detail::get_internals().registered_exception_translators;
        for (auto &translator: registered_exception_translators)
        {
            try
            {
                pybind11::print("FOOBAR 1");
                translator(ex);
                pybind11::print("FOOBAR 2");
            }
            catch (std::exception &e)
            {
                ex = std::current_exception();
                continue;
            }
            set = true;
            break;
        }
    }

    if (!set)
        return pybind11::none();

    return py::error_already_set().value();
}

}

/** IDEA: awaitable<py::object>
 *  type-caster goes both ways here, keeps the value_type around for casting back.
 *
 */

namespace pybind11::detail
{

template <>
struct type_caster< std::exception_ptr >
{
  public:
  PYBIND11_TYPE_CASTER(std::exception_ptr, _("exception_ptr"));

    bool
    load(const handle & , bool b)
    {
        return false;
    }

    static handle
    cast(std::exception_ptr ex, return_value_policy /* policy */, handle /* parent */)
    {
        if (!ex)
            return none().release();
        else
            return asio_py::detail::translate_exception(ex).release();

    }
};

template <>
struct type_caster< std::tuple< std::exception_ptr, object > >
{
  public:
    using tuple_t = std::tuple< std::exception_ptr, object >;
    PYBIND11_TYPE_CASTER(tuple_t, _("Tuple[std.exception_ptr, Tuple]"));

    bool
    load(const handle & , bool b)
    {
        return false;
    }

    static handle
    cast(tuple_t tup, return_value_policy policy, handle parent)
    {
        tuple result(2);
        PyTuple_SetItem(
                result.ptr(), 0,
                reinterpret_steal<object>(make_caster<std::exception_ptr>::cast(get<0>(tup), policy, parent)).release().ptr());

        PyTuple_SetItem(
                result.ptr(), 1,
                get<1>(tup).release().ptr());
        return result.release();
    }
};


}   // namespace pybind11::detail


#endif //ASIO_PY_EXCEPTIONS_HPP
