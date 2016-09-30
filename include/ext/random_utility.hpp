/*
 * Utilities for random number generation.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_RANDOM_UTILITY_HPP
#define EXT_RANDOM_UTILITY_HPP

#include <algorithm>
#include <array>
#include <limits>
#include <random>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace ext
{
    /**
     * Checks if a type satisfies SeedSequence requirements.
     */
    template<typename T>
    struct is_seed_sequence;

    namespace detail
    {
        template<typename T,
                 typename R = typename T::result_type,
                 typename S = decltype(std::declval<T&>().size()),
                 typename G = decltype(std::declval<T&>().generate(
                                            std::declval<R*>(),
                                            std::declval<R*>()))>
        std::true_type probe_seed_sequence(int);

        template<typename T>
        std::false_type probe_seed_sequence(...);
    }

    template<typename T>
    struct is_seed_sequence : decltype(detail::probe_seed_sequence<T>(0))
    {
    };

    /**
     * Fills array with seed values.
     */
    template<typename Seed, typename T, std::size_t N>
    void seed_state(Seed& seed, std::array<T, N>& state)
    {
        static_assert(ext::is_seed_sequence<Seed>::value,
                      "seed is expected to be a SeedSequence");

        static_assert(std::is_unsigned<T>::value,
                      "state is expected to be array of unsigned integers");

        // Note: Conforming seed sequence generates 32-bit unsigned integers.
        constexpr std::size_t word_bits = std::numeric_limits<T>::digits;
        constexpr std::size_t seed_per_word = (word_bits + 31) / 32;
        constexpr std::size_t shift_bits = std::min<std::size_t>(word_bits, 32);

        std::array<typename Seed::result_type, N * seed_per_word> seeds;
        seed.generate(seeds.begin(), seeds.end());

        for (std::size_t i = 0; i < N; ++i)
        {
            state[i] = 0;
            for (std::size_t j = 0; j < seed_per_word; ++j)
            {
                state[i] = T(state[i] << 1 << (shift_bits - 1));
                state[i] = T(state[i] | seeds[i * seed_per_word + j]);
            }
        }
    }
}

#endif
