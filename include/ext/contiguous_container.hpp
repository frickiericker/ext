/*
 * Handling containers that have underlying contiguous storage.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_CONTIGUOUS_CONTAINER_HPP
#define EXT_CONTIGUOUS_CONTAINER_HPP

#include <type_traits>
#include <utility>
#include <cstddef>

namespace ext
{
    /**
     * Checks if a type exposes access to contiguous memory region.
     *
     * Provides the member constant `value` which equals to `true` if and only
     * if `C` is a class type with member functions `C::data()` and `C::size()`
     * where `data()` returns a pointer and `size()` returns an integer.
     */
    template<typename C>
    struct has_contiguous_access;

    namespace detail
    {
        template<typename C,
                 typename Data = decltype(std::declval<C>().data()),
                 typename Size = decltype(std::declval<C>().size()),
                 typename = std::enable_if_t<std::is_pointer<Data>::value>,
                 typename = std::enable_if_t<std::is_integral<Size>::value>>
        std::true_type probe_contiguous_access(int);

        template<typename C>
        std::false_type probe_contiguous_access(...);
    }

    template<typename C>
    struct has_contiguous_access
        : decltype(detail::probe_contiguous_access<C>(0))
    {
    };

    /**
     * Obtains the type used to access an element of a contiguous container.
     *
     * If `ext::has_contiguous_access<C>::value` is true, this trait provides
     * the member type `type` aliased to the type of value pointed to by the
     * result of `C::data()`.
     *
     * This trait is SFINAE friendly.
     */
    template<typename C>
    struct contiguous_access_type;

    namespace detail
    {
        template<typename C, bool = ext::has_contiguous_access<C>::value>
        struct contiguous_access_type_impl
        {
        };

        template<typename C>
        struct contiguous_access_type_impl<C, true>
        {
            using type =
                std::remove_pointer_t<decltype(std::declval<C>().data())>;
        };
    }

    template<typename C>
    struct contiguous_access_type : detail::contiguous_access_type_impl<C>
    {
    };

    template<typename C>
    using contiguous_access_type_t =
        typename ext::contiguous_access_type<C>::type;

    /**
     * Returns a pointer to the first element of a contiguous container or
     * array.
     */
    template<typename C>
    ext::contiguous_access_type_t<C>* begin_ptr(C& container)
    {
        return container.data();
    }

    template<typename T, std::size_t N>
    constexpr
    T* begin_ptr(T(& array)[N]) noexcept
    {
        return array;
    }

    /**
     * Returns a pointer to the past the last element of a contiguous container
     * or array.
     */
    template<typename C>
    ext::contiguous_access_type_t<C>* end_ptr(C& container)
    {
        return container.data() + container.size();
    }

    template<typename T, std::size_t N>
    constexpr
    T* end_ptr(T(& array)[N]) noexcept
    {
        return array + N;
    }
}

#endif
