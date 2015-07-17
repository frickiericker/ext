#ifndef EXT_RANDOM_HPP
#define EXT_RANDOM_HPP

#include <algorithm>
#include <array>
#include <iterator>
#include <limits>
#include <random>
#include <type_traits>
#include <utility>

#include <cmath>
#include <cstddef>
#include <cstdint>

#include "meta.hpp"


//____ Metafunction __________________________________________________________

namespace ext
{
    namespace detail
    {
        template<typename S>
        auto try_seed_seq_generate(
                S&             q  = std::declval<S&>(),
                unsigned long* ib = std::declval<unsigned long*>(),
                unsigned long* ie = std::declval<unsigned long*>()
                )
            -> decltype(q.generate(ib, ie));

        template<typename T>
        using seed_seq_generate_t = decltype(try_seed_seq_generate<T>());
    }

    /*
     * Determines whether T satisfies the seed sequence requirements.
     */
    template<typename T>
    struct is_seed_sequence : n4502::is_detected<detail::seed_seq_generate_t, T>
    {
    };
}

//____ Engine ________________________________________________________________

namespace ext
{
    /*
     * TODO
     */
    template<typename Derived, typename T>
    struct random_engine_facade
    {
        using derived_type = Derived;
        using result_type = T;

        // ____ Constructors and seeding functions ___________________________

        random_engine_facade()
        {
            derived().seed();
        }

        explicit
        random_engine_facade(result_type s)
        {
            derived().seed(s);
        }

        template< typename SeedSeq
                , std::enable_if_t<is_seed_sequence<SeedSeq>::value, int> = 0>
        explicit
        random_engine_facade(SeedSeq& s)
        {
            derived().seed(s);
        }

        void seed()
        {
            derived().seed(derived_type::default_seed);
        }

        void seed(result_type s)
        {
            std::seed_seq seq{s};
            derived().seed(seq);
        }

        /* TODO
        template< typename SeedSeq
                , std::enable_if_t<is_seed_sequence<SeedSeq>::value, int> = 0>
        void seed(SeedSeq& s)
        {
            using std::begin;
            using std::end;
        }
        */

        // ____ Random number generation _____________________________________

        result_type operator()()
        {
            derived().advance();
            return derived().value();
        }

        void discard(unsigned long long z)
        {
            derived().advance(z);
        }

        // ____ Engine state _________________________________________________

        /* TODO
        bool operator==(random_engine_facade const& other) const
        {
            return derived().state() == other.derived().state();
        }

        bool operator!=(random_engine_facade const& other) const
        {
            return !(*this == other);
        }

        template<typename Char, typename CharTraits>
        friend
        std::basic_ostream<Char, CharTraits>&
        operator<<(std::basic_ostream<Char, CharTraits>& output_stream,
                   random_engine_facade const& engine)
        {
            return output_stream;
        }

        template<typename Char, typename CharTraits>
        friend
        std::basic_istream<Char, CharTraits>&
        operator>>(std::basic_istream<Char, CharTraits>& input_stream,
                   random_engine_facade& engine)
        {
            return input_stream;
        }
        */

        // ____ CRTP _________________________________________________________

        void advance(unsigned long long z)
        {
            for (auto i = 0uLL; i < z; ++i)
                derived().advance();
        }

      private:

        derived_type& derived() noexcept
        {
            return static_cast<derived_type&>(*this);
        }

        derived_type const& derived() const noexcept
        {
            return static_cast<derived_type const&>(*this);
        }
    };
}

//____ Algorithm _____________________________________________________________

namespace ext
{
    template<typename T, std::size_t bits = std::numeric_limits<T>::digits / 2,
             typename RNG>
    T generate_normal(RNG& random_engine)
    {
        using std::abs;
        using std::log;
        using std::sqrt;

        auto const v_factor = T(sqrt(2 / M_E) * 2);

        for (;;)
        {
            auto u = std::generate_canonical<T, bits>(random_engine);
            auto v = std::generate_canonical<T, bits>(random_engine);

            v = v_factor * (v - T(0.5));

            // Numerical Recipes here.
            //
            // Use certain elliptic curves enclosing and covered by the
            // acceptance region to reduce necessity of calculating the
            // exact boundary of the region.
            //
            // NOTE: 1.9x speedup on IvyBridge. Without this optimization
            // ratio-of-uniforms performs worse than cached Box-Muller.

            auto x = u - T(0.449871);
            auto y = abs(v) + T(0.386595);
            auto q = x*x + y*(T(0.19600)*y - T(0.25472)*x);

            if (q > T(0.27597) && (q > T(0.27846) || v*v > -4 * (u*u) * log(u)))
                continue;

            return v / u;
        }
    }
}

#endif
