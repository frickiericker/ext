#ifndef EXT_BITHACK_HPP
#define EXT_BITHACK_HPP

#include <array>


namespace ext
{
    constexpr
    unsigned spread_bits_3(unsigned x)
    {
        x = (x | (x << 16)) & 0x030000FF;
        x = (x | (x <<  8)) & 0x0300F00F;
        x = (x | (x <<  4)) & 0x030C30C3;
        x = (x | (x <<  2)) & 0x09249249;
        return x;
    }

    constexpr
    unsigned compress_bits_3(unsigned x)
    {
        x =  x              & 0x09249249;
        x = (x | (x >>  2)) & 0x030C30C3;
        x = (x | (x >>  4)) & 0x0300F00F;
        x = (x | (x >>  8)) & 0x030000FF;
        x = (x | (x >> 16)) & 0x000003FF;
        return x;
    }

    constexpr
    unsigned morton_encode_3(unsigned x, unsigned y, unsigned z)
    {
        // Clang with -fslp-vectorize-aggressive successfully auto-vectorizes this expression.
        return  spread_bits_3(x)
             | (spread_bits_3(y) << 1)
             | (spread_bits_3(z) << 2);
    }

    constexpr
    std::array<unsigned, 3> morton_decode_3(unsigned code)
    {
        return {{compress_bits_3(code     ),
                 compress_bits_3(code >> 1),
                 compress_bits_3(code >> 2)}};
    }
}

#endif
