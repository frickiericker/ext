#include <iterator>
#include <forward_list>
#include <type_traits>
#include <vector>
#include <cstddef>

#include <catch.hpp>

#include <ext/iterator.hpp>


// Integral Range

TEST_CASE("ext::range (integral) / basic usage")
{
    auto r1 = ext::range(5);
    CHECK(r1.size() == std::size_t(5));
    auto i1 = r1.begin();
    CHECK(*i1 == 0);
    ++i1;
    CHECK(*i1 == 1);
    ++i1;
    CHECK(*i1 == 2);
    ++i1;
    CHECK(*i1 == 3);
    ++i1;
    CHECK(*i1 == 4);
    ++i1;
    CHECK(i1 == r1.end());

    auto r2 = ext::range(2, 6);
    CHECK(r2.size() == std::size_t(4));
    auto i2 = r2.begin();
    CHECK(*i2 == 2);
    ++i2;
    CHECK(*i2 == 3);
    ++i2;
    CHECK(*i2 == 4);
    ++i2;
    CHECK(*i2 == 5);
    ++i2;
    CHECK(i2 == r2.end());
}

TEST_CASE("ext::range (integral) / degenerate cases")
{
    auto r1 = ext::range(0);
    CHECK(r1.size() == std::size_t(0));
    CHECK(r1.begin() == r1.end());

    auto r2 = ext::range(5, 5);
    CHECK(r2.size() == std::size_t(0));
    CHECK(r2.begin() == r2.end());
}

TEST_CASE("ext::range (integral) / explicit type")
{
    auto r1 = ext::range<signed char>(15);
    CHECK(r1.size() == std::size_t(15));
    auto i1 = r1.begin();
    signed char r1_typecheck = *i1;
    CHECK(r1_typecheck == static_cast<signed char>(0));

    auto r2 = ext::range<signed char>(-5, 5);
    CHECK(r2.size() == std::size_t(10));
    auto i2 = r2.begin();
    signed char r2_typecheck = *i2;
    CHECK(r2_typecheck == static_cast<signed char>(-5));
}

TEST_CASE("ext::range (integral) / random access")
{
    auto r1 = ext::range<long>(1000000L);
    CHECK(r1.size() == std::size_t(1000000));
    auto i1 = r1.begin();
    i1 += 999999;
    CHECK(*i1 == 999999);
    ++i1;
    CHECK(i1 == r1.end());
    i1 -= 100000;
    CHECK(*i1 == 900000);
    --i1;
    CHECK(*i1 == 899999);
}

TEST_CASE("ext::range (integral) / positive step")
{
    auto r1 = ext::range(3, 10, 2);
    CHECK(r1.size() == std::size_t(4));
    auto i1 = r1.begin();
    CHECK(*i1 == 3);
    ++i1;
    CHECK(*i1 == 5);
    ++i1;
    CHECK(*i1 == 7);
    ++i1;
    CHECK(*i1 == 9);
    ++i1;
    CHECK(i1 == r1.end());

    auto r2 = ext::range(1, 2, 3);
    CHECK(r2.size() == std::size_t(1));
    auto i2 = r2.begin();
    CHECK(*i2 == 1);
    ++i2;
    CHECK(i2 == r2.end());
}

TEST_CASE("ext::range (integral) / negative step")
{
    auto r1 = ext::range(5, 2, -1);
    CHECK(r1.size() == std::size_t(3));
    auto i1 = r1.begin();
    CHECK(*i1 == 5);
    ++i1;
    CHECK(*i1 == 4);
    ++i1;
    CHECK(*i1 == 3);
    ++i1;
    CHECK(i1 == r1.end());
}

// Iterator Range

TEST_CASE("ext::range (iterator) / random access")
{
    auto v1 = std::vector<int> {1, 2, 3};
    auto r1 = ext::range(v1.begin(), v1.end());
    auto i1 = r1.begin();
    CHECK(&*i1 == &v1[0]);
    ++i1;
    CHECK(&*i1 == &v1[1]);
    i1 += 2;
    CHECK(i1 == r1.end());
}

TEST_CASE("ext::range (iterator) / forward access")
{
    auto l1 = std::forward_list<int> {1, 2, 3};
    auto r1 = ext::range(l1.begin(), l1.end());
    auto i1 = r1.begin();
    CHECK(*i1 == 1);
    ++i1;
    CHECK(*i1 == 2);
    ++i1;
    CHECK(*i1 == 3);
    ++i1;
    CHECK(i1 == r1.end());
}

TEST_CASE("ext::range (iterator) / positive step")
{
    auto v1 = std::vector<int> {1, 2, 3, 4};
    auto r1 = ext::range(v1.begin(), v1.end(), 2);
    auto i1 = r1.begin();
    CHECK(*i1 == 1);
    ++i1;
    CHECK(*i1 == 3);
    ++i1;
    CHECK(i1 == r1.end());

    auto l2 = std::forward_list<int> {1, 2, 3, 4, 5, 6, 7};
    auto r2 = ext::range(l2.begin(), l2.end(), 3);
    auto i2 = r2.begin();
    CHECK(*i2 == 1);
    ++i2;
    CHECK(*i2 == 4);
    ++i2;
    CHECK(*i2 == 7);
    ++i2;
    CHECK(i2 == r2.end());
}
