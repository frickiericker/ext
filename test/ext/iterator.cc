#include <iterator>
#include <forward_list>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <ext/iterator.hpp>


// Integral Range

TEST(IntRange, BasicUse)
{
    auto r1 = ext::range(5);
    ASSERT_EQ(r1.size(), 5);
    auto i1 = r1.begin();
    ASSERT_EQ(*i1, 0);
    ++i1;
    ASSERT_EQ(*i1, 1);
    ++i1;
    ASSERT_EQ(*i1, 2);
    ++i1;
    ASSERT_EQ(*i1, 3);
    ++i1;
    ASSERT_EQ(*i1, 4);
    ++i1;
    ASSERT_EQ(i1, r1.end());

    auto r2 = ext::range(2, 6);
    ASSERT_EQ(r2.size(), 4);
    auto i2 = r2.begin();
    ASSERT_EQ(*i2, 2);
    ++i2;
    ASSERT_EQ(*i2, 3);
    ++i2;
    ASSERT_EQ(*i2, 4);
    ++i2;
    ASSERT_EQ(*i2, 5);
    ++i2;
    ASSERT_EQ(i2, r2.end());
}

TEST(IntRange, DegenerateCases)
{
    auto r1 = ext::range(0);
    ASSERT_EQ(r1.size(), 0);
    ASSERT_EQ(r1.begin(), r1.end());

    auto r2 = ext::range(5, 5);
    ASSERT_EQ(r2.size(), 0);
    ASSERT_EQ(r2.begin(), r2.end());
}

TEST(IntRange, ExplicitType)
{
    auto r1 = ext::range<signed char>(15);
    ASSERT_EQ(r1.size(), 15);
    auto i1 = r1.begin();
    signed char r1_typecheck = *i1;
    ASSERT_EQ(r1_typecheck, 0);

    auto r2 = ext::range<signed char>(-5, 5);
    ASSERT_EQ(r2.size(), 10);
    auto i2 = r2.begin();
    signed char r2_typecheck = *i2;
    ASSERT_EQ(r2_typecheck, -5);
}

TEST(IntRange, RandomAccess)
{
    auto r1 = ext::range<long>(1000000L);
    ASSERT_EQ(r1.size(), 1000000);
    auto i1 = r1.begin();
    i1 += 999999;
    ASSERT_EQ(*i1, 999999);
    ++i1;
    ASSERT_EQ(i1, r1.end());
    i1 -= 100000;
    ASSERT_EQ(*i1, 900000);
    --i1;
    ASSERT_EQ(*i1, 899999);
}

TEST(IntRange, Step)
{
    auto r1 = ext::range(3, 10, 2);
    ASSERT_EQ(r1.size(), 4);
    auto i1 = r1.begin();
    ASSERT_EQ(*i1, 3);
    ++i1;
    ASSERT_EQ(*i1, 5);
    ++i1;
    ASSERT_EQ(*i1, 7);
    ++i1;
    ASSERT_EQ(*i1, 9);
    ++i1;
    ASSERT_EQ(i1, r1.end());

    auto r2 = ext::range(1, 2, 3);
    ASSERT_EQ(r2.size(), 1);
    auto i2 = r2.begin();
    ASSERT_EQ(*i2, 1);
    ++i2;
    ASSERT_EQ(i2, r2.end());
}

TEST(IntRange, NegativeStep)
{
    auto r1 = ext::range(5, 2, -1);
    ASSERT_EQ(r1.size(), 3);
    auto i1 = r1.begin();
    ASSERT_EQ(*i1, 5);
    ++i1;
    ASSERT_EQ(*i1, 4);
    ++i1;
    ASSERT_EQ(*i1, 3);
    ++i1;
    ASSERT_EQ(i1, r1.end());
}

// Iterator Range

TEST(IteratorRange, RandomAccess)
{
    auto v1 = std::vector<int> {1, 2, 3};
    auto r1 = ext::range(v1.begin(), v1.end());
    auto i1 = r1.begin();
    ASSERT_EQ(&*i1, &v1[0]);
    ++i1;
    ASSERT_EQ(&*i1, &v1[1]);
    i1 += 2;
    ASSERT_EQ(i1, r1.end());
}

TEST(IteratorRange, ForwardAccess)
{
    auto l1 = std::forward_list<int> {1, 2, 3};
    auto r1 = ext::range(l1.begin(), l1.end());
    auto i1 = r1.begin();
    ASSERT_EQ(*i1, 1);
    ++i1;
    ASSERT_EQ(*i1, 2);
    ++i1;
    ASSERT_EQ(*i1, 3);
    ++i1;
    ASSERT_EQ(i1, r1.end());
}

TEST(IteratorRange, Step)
{
    auto v1 = std::vector<int> {1, 2, 3, 4};
    auto r1 = ext::range(v1.begin(), v1.end(), 2);
    auto i1 = r1.begin();
    ASSERT_EQ(*i1, 1);
    ++i1;
    ASSERT_EQ(*i1, 3);
    ++i1;
    ASSERT_EQ(i1, r1.end());

    auto l2 = std::forward_list<int> {1, 2, 3, 4, 5, 6, 7};
    auto r2 = ext::range(l2.begin(), l2.end(), 3);
    auto i2 = r2.begin();
    ASSERT_EQ(*i2, 1);
    ++i2;
    ASSERT_EQ(*i2, 4);
    ++i2;
    ASSERT_EQ(*i2, 7);
    ++i2;
    ASSERT_EQ(i2, r2.end());
}
