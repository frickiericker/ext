#ifndef EXT_RANDOM_HPP
#define EXT_RANDOM_HPP

#include <limits>
#include <random>

#include <cstddef>
#include <cmath>


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

            auto x = u - 0.449871;
            auto y = abs(v) + 0.386595;
            auto q = x*x + y*(0.19600*y - 0.25472*x);

            if (q > 0.27597 && (q > 0.27846 || v*v > -4 * (u*u) * log(u)))
                continue;

            return v / u;
        }
    }
}

#endif
