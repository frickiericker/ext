/*
 * Generic metafunctions.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_TYPE_TRAITS_HPP
#define EXT_TYPE_TRAITS_HPP

#include <type_traits>

namespace ext
{
    /**
     * Identity metafunction.
     *
     * === Use case ===
     *
     * `ext::identity` can be used in parameter type in function template
     * declaration to prevent deduction like this:
     *
     * ```
     * template<typename T>
     * void assign(T& var, ext::identity_t<T> const& value)
     * {
     *     var = value;
     * }
     * ```
     *
     * Here the template type parameter `T` is deduced sorely from the first
     * argument `var`, eliminates possible ambiguity.
     */
    template<typename T>
    struct identity
    {
        using type = T;
    };

    template<typename T>
    using identity_t = typename identity<T>::type;

    /**
     * Checks if a type is less const qualified type of the other one.
     *
     * Provides the member constant `value` which equals to `true` if and only
     * if `T` and `U` are the same type modulo const qualification and `T` is
     * no more const qualified than `U`. This means that `T*` is implicitly
     * convertible to `U*` (without upcast for class types).
     */
    template<typename T, typename U>
    struct is_const_convertible;

    template<typename, typename>
    struct is_const_convertible : std::false_type
    {
    };

    template<typename T>
    struct is_const_convertible<T, T> : std::true_type
    {
    };

    template<typename T>
    struct is_const_convertible<T, T const> : std::true_type
    {
    };
}

#endif
