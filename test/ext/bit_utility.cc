#include <bitset>
#include <cstddef>
#include <cstdint>

#include <catch.hpp>

#include <ext/bit_utility.hpp>


TEST_CASE("ext::rotate - 16 bits")
{
    using std::uint16_t;

    auto bits = [](char const(& pattern)[17])
    {
        return static_cast<uint16_t>(std::bitset<16>(pattern).to_ulong());
    };

    uint16_t const n = bits("1101111010101101");

    CHECK(ext::rotate(n,  0) == bits("1101111010101101"));
    CHECK(ext::rotate(n,  1) == bits("1011110101011011"));
    CHECK(ext::rotate(n,  2) == bits("0111101010110111"));
    CHECK(ext::rotate(n,  3) == bits("1111010101101110"));
    CHECK(ext::rotate(n,  4) == bits("1110101011011101"));
    CHECK(ext::rotate(n,  5) == bits("1101010110111011"));
    CHECK(ext::rotate(n,  6) == bits("1010101101110111"));
    CHECK(ext::rotate(n,  7) == bits("0101011011101111"));
    CHECK(ext::rotate(n,  8) == bits("1010110111011110"));
    CHECK(ext::rotate(n,  9) == bits("0101101110111101"));
    CHECK(ext::rotate(n, 10) == bits("1011011101111010"));
    CHECK(ext::rotate(n, 11) == bits("0110111011110101"));
    CHECK(ext::rotate(n, 12) == bits("1101110111101010"));
    CHECK(ext::rotate(n, 13) == bits("1011101111010101"));
    CHECK(ext::rotate(n, 14) == bits("0111011110101011"));
    CHECK(ext::rotate(n, 15) == bits("1110111101010110"));
}
