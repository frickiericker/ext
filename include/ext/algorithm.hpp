/*
 * Generic algorithms.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_ALGORITHM_HPP
#define EXT_ALGORITHM_HPP

#include <algorithm>

namespace ext
{
    /*
     * Generates n successive increments of the given value.
     */
    template<typename OutputIterator, typename Size, typename T>
    OutputIterator iota_n(OutputIterator out, Size n, T value)
    {
        return std::generate_n(out, n, [&]() { return value++; })
    }
}

#endif
