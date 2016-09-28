/*
 * Utility functions for numbers.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_NUMERIC_UTILITY_HPP
#define EXT_NUMERIC_UTILITY_HPP

#include <cmath>

#include "type_traits.hpp"

namespace ext
{
    /**
     * Trait to get special values of numeric type.
     */
    template<typename T>
    struct special_values
    {
        /**
         * Returns zero, or additive unity, of type `T`.
         */
        static constexpr
        T zero()
        {
            return T {};
        }

        /**
         * Returns one, or multiplicative unity, of type `T`.
         */
        static constexpr
        T one()
        {
            return T {1};
        }
    };

    /**
     * Clamps a value with specified lower and upper bounds.
     */
    template<typename T>
    constexpr
    T clamp(T const& value,
            ext::identity_t<T> const& min,
            ext::identity_t<T> const& max)
    {
        return value < min ? min :
               value > max ? max : value;
    }

    /**
     * Calculates number raised to constant exponent.
     */
    template<unsigned N, typename T>
    constexpr
    T pow(T const& x)
    {
        if (N == 0) {
            return ext::special_values<T>::one();
        }

        // Let compiler optimize this.
        T result {x};
        for (unsigned i = 1; i < N; ++i) {
            result *= x;
        }
        return result;
    }
}

#endif
