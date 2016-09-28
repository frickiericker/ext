#include <random>
#include <sstream>

#include <catch.hpp>

#include <ext/small_fast_counting_engine_v4.hpp>


TEST_CASE("ext::sfc64 - engine characteristics")
{
    constexpr ext::sfc64::result_type min = ext::sfc64::min();
    constexpr ext::sfc64::result_type max = ext::sfc64::max();
    CHECK(min == 0);
    CHECK(max == 0xffffffffffffffffu);
}

TEST_CASE("ext::sfc64 - seeding constructor")
{
    SECTION("default")
    {
        ext::sfc64 engine;
    }

    SECTION("numeric")
    {
        ext::sfc64 engine {1234};
    }

    SECTION("seed_seq")
    {
        std::seed_seq seed;
        ext::sfc64 engine {seed};
    }
}

TEST_CASE("ext::sfc64 - seeding")
{
    ext::sfc64 engine;

    SECTION("default")
    {
        engine.seed();
        CHECK(engine == (ext::sfc64 {}));
    }

    SECTION("numeric")
    {
        engine.seed(1234);
        CHECK(engine == (ext::sfc64 {1234}));
    }

    SECTION("seed_seq")
    {
        std::seed_seq seed;
        engine.seed(seed);
        CHECK(engine == (ext::sfc64 {seed}));
    }
}

TEST_CASE("ext::sfc16/32/64 - random number generation")
{
    ext::sfc16 sfc16;
    ext::sfc32 sfc32;
    ext::sfc64 sfc64;

    SECTION("first four numbers")
    {
        CHECK(sfc16() == 19789u);
        CHECK(sfc16() == 56786u);
        CHECK(sfc16() == 31714u);
        CHECK(sfc16() == 60293u);

        CHECK(sfc32() == 3420751835u);
        CHECK(sfc32() == 3003311523u);
        CHECK(sfc32() == 2455794221u);
        CHECK(sfc32() == 1063818521u);

        CHECK(sfc64() == 6361293506195189846u);
        CHECK(sfc64() == 2957386357819892447u);
        CHECK(sfc64() == 13267547607742811100u);
        CHECK(sfc64() == 6537049622224303171u);
    }

    SECTION("1000000-th number")
    {
        sfc16.discard(999999);
        sfc32.discard(999999);
        sfc64.discard(999999);
        CHECK(sfc16() == 44098u);
        CHECK(sfc32() == 945273761u);
        CHECK(sfc64() == 2254566973870142647u);
    }
}

TEST_CASE("ext::sfc64 - comparison operators")
{
    ext::sfc64 const engine_1 {1234};
    ext::sfc64 const engine_2 {1234};
    ext::sfc64 const engine_3 {4321};
    CHECK(engine_1 == engine_2);
    CHECK(engine_1 != engine_3);
    CHECK(engine_2 == engine_1);
    CHECK(engine_3 != engine_1);
}

TEST_CASE("ext::sfc64 - stream I/O")
{
    ext::sfc64 const src {1234};
    ext::sfc64 dst;
    CHECK(src != dst);

    // Transfer state via stream.
    std::ostringstream os;
    os << src;
    std::istringstream is {os.str()};
    is >> dst;
    CHECK(src == dst);
}
