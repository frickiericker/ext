#ifndef EXT_META_HPP
#define EXT_META_HPP

#include <type_traits>


namespace ext
{
    template<typename T>
    struct make_non_deduced
    {
        using type = T;
    };

    template<typename T>
    using make_non_deduced_t = typename make_non_deduced<T>::type;
}

//___ N4502 detection idiom __________________________________________________

namespace ext { inline namespace n4502
{
    namespace detail
    {
        template<typename...>
        struct voider
        {
            using type = void;
        };
    }

    /*
     * Metafunction that maps any number of well-formed types Args... to void.
     */
    template<typename... Args>
    using void_t = typename detail::voider<Args...>::type;

    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };

    namespace detail
    {
        template<typename Default, typename AlwaysVoid,
                 template<typename...> class Op, typename... Args>
        struct detector
        {
            using value_t = std::false_type;
            using type = Default;
        };

        template<typename Default, template<typename...> class Op, typename... Args>
        struct detector<Default, void_t<Op<Args...>>, Op, Args...>
        {
            using value_t = std::true_type;
            using type = Op<Args...>;
        };
    }

    /*
     * Returns std::true_type if Op<Args...> is well-formed or std::false_type
     * otherwise.
     */
    template<template<typename> class Op, typename... Args>
    using is_detected = typename detail::detector<nonesuch, void , Op, Args...>::value_t;

    /*
     * Returns Op<Args...> if it's well-formed or nonesuch otherwise.
     */
    template<template<typename> class Op, typename... Args>
    using detected_t = typename detail::detector<nonesuch, void , Op, Args...>::type;
}}

#endif
