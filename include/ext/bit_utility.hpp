/*
 * Utilities for trnasforming bit pattern of unsigned integer.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_BIT_UTILITY_HPP
#define EXT_BIT_UTILITY_HPP

#include <limits>
#include <type_traits>

namespace ext
{
    /**
     * Rotates bits to left.
     *
     * Behaviour is undefined if the rotation amount is greater than the bit
     * width of the integral type.
     */
    template<typename T,
             std::enable_if_t<std::is_integral<T>::value, int> = 0>
    constexpr
    T rotate(T x, unsigned n)
    {
        constexpr auto bits = std::numeric_limits<T>::digits;

        if (n == 0)
            return x;
        else
            return T(x << n) | T(x >> (bits - n));
    }
}

#endif
