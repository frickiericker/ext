#ifndef EXT_UTILITY
#define EXT_UTILITY

#include <type_traits>

namespace ext
{
    /**
     * Convertes reference to const-reference.
     */
    template<typename T>
    constexpr
    T const& as_const(T const& x) noexcept
    {
        return x;
    }
}

namespace ext
{
    /**
     * Returns the smallest multiple of b that is no less than a.
     */
    template< typename I
            , std::enable_if_t<std::is_integral<T>::value, int> = 0>
    constexpr
    I round_up(I a, I b)
    {
        return (a + b - I(1)) / b * b;
    }
}

#endif
