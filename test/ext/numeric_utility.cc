#include <catch.hpp>

#include <ext/numeric_utility.hpp>


TEST_CASE("ext::special_values")
{
    CHECK(ext::special_values<int>::zero() == 0);
    CHECK(ext::special_values<int>::one() == 1);

    // Narrower type
    CHECK(ext::special_values<char>::zero() == char(0));
    CHECK(ext::special_values<char>::one() == char(1));

    // Floating-point
    CHECK(ext::special_values<float>::zero() == 0.0F);
    CHECK(ext::special_values<float>::one() == 1.0F);
    CHECK(ext::special_values<double>::zero() == 0.0);
    CHECK(ext::special_values<double>::one() == 1.0);
}

TEST_CASE("ext::clamp")
{
    // int
    CHECK(ext::clamp(42, 0, 100) == 42);
    CHECK(ext::clamp(0, 0, 100) == 0);
    CHECK(ext::clamp(100, 0, 100) == 100);
    CHECK(ext::clamp(-10, 0, 100) == 0);
    CHECK(ext::clamp(128, 0, 100) == 100);

    // double
    CHECK(ext::clamp(0.5, 0.1, 0.9) == 0.5);
    CHECK(ext::clamp(0.1, 0.1, 0.9) == 0.1);
    CHECK(ext::clamp(0.9, 0.1, 0.9) == 0.9);
    CHECK(ext::clamp(0.0, 0.1, 0.9) == 0.1);
    CHECK(ext::clamp(1.0, 0.1, 0.9) == 0.9);
}

TEST_CASE("ext::pow")
{
    // int
    CHECK(ext::pow<0>(2) == 1);
    CHECK(ext::pow<1>(2) == 2);
    CHECK(ext::pow<2>(2) == 4);
    CHECK(ext::pow<3>(2) == 8);
    CHECK(ext::pow<4>(2) == 16);
    CHECK(ext::pow<5>(2) == 32);
    CHECK(ext::pow<6>(2) == 64);
    CHECK(ext::pow<7>(2) == 128);
    CHECK(ext::pow<8>(2) == 256);
    CHECK(ext::pow<9>(2) == 512);
    CHECK(ext::pow<10>(2) == 1024);
    CHECK(ext::pow<11>(2) == 2048);
    CHECK(ext::pow<12>(2) == 4096);
    CHECK(ext::pow<13>(2) == 8192);
    CHECK(ext::pow<14>(2) == 16384);

    // double
    CHECK(ext::pow<0>(0.1) == Approx(1.0));
    CHECK(ext::pow<1>(0.1) == Approx(1e-1));
    CHECK(ext::pow<2>(0.1) == Approx(1e-2));
    CHECK(ext::pow<3>(0.1) == Approx(1e-3));
    CHECK(ext::pow<4>(0.1) == Approx(1e-4));
    CHECK(ext::pow<5>(0.1) == Approx(1e-5));
    CHECK(ext::pow<6>(0.1) == Approx(1e-6));
    CHECK(ext::pow<7>(0.1) == Approx(1e-7));
    CHECK(ext::pow<8>(0.1) == Approx(1e-8));
    CHECK(ext::pow<9>(0.1) == Approx(1e-9));
}
