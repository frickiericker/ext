#include <array>
#include <random>

#include <cstdint>

#include <catch.hpp>

#include <ext/random_utility.hpp>


TEST_CASE("ext::is_seed_sequence")
{
    CHECK((ext::is_seed_sequence<std::seed_seq>::value));

    CHECK_FALSE((ext::is_seed_sequence<std::minstd_rand>::value));
    CHECK_FALSE((ext::is_seed_sequence<int>::value));
    CHECK_FALSE((ext::is_seed_sequence<void>::value));
}

TEST_CASE("ext::seed_state")
{
    SECTION("exact word size")
    {
        std::array<std::uint32_t, 4> state {{}};
        std::seed_seq seed;
        ext::seed_state(seed, state);
        CHECK(state[0] != 0u);
        CHECK(state[1] != 0u);
        CHECK(state[2] != 0u);
        CHECK(state[3] != 0u);
    }

    SECTION("narrow word")
    {
        std::array<std::uint16_t, 4> state {{}};
        std::seed_seq seed;
        ext::seed_state(seed, state);
        CHECK(state[0] != 0u);
        CHECK(state[1] != 0u);
        CHECK(state[2] != 0u);
        CHECK(state[3] != 0u);
    }

    SECTION("wide word")
    {
        std::array<std::uint64_t, 4> state {{}};
        std::seed_seq seed;
        ext::seed_state(seed, state);
        CHECK(state[0] > 0xffffffffu);
        CHECK(state[1] > 0xffffffffu);
        CHECK(state[2] > 0xffffffffu);
        CHECK(state[3] > 0xffffffffu);
    }
}
