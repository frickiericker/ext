#include <array>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include <ext/array_view.hpp>


TEST(ArrayView, Factory)
{
    // Array
    int array[] = {1234, 5678};
    auto av_array = ext::make_array_view(array);
    ASSERT_EQ(av_array.data(), array);
    ASSERT_EQ(av_array.size(), 2);
    ASSERT_TRUE((std::is_same<decltype(av_array), ext::array_view<int>>::value));

    // std::array
    auto stdarray = std::array<int, 3> {{1234, 5678, 9012}};
    auto av_stdarray = ext::make_array_view(stdarray);
    ASSERT_EQ(av_stdarray.data(), stdarray.data());
    ASSERT_EQ(av_stdarray.size(), stdarray.size());
    ASSERT_TRUE((std::is_same<decltype(av_stdarray), ext::array_view<int>>::value));

    // std::vector
    auto stdvec = std::vector<int> {12, 34, 56, 78, 90};
    auto av_stdvec = ext::make_array_view(stdvec);
    ASSERT_EQ(av_stdvec.data(), stdvec.data());
    ASSERT_EQ(av_stdvec.size(), stdvec.size());
    ASSERT_TRUE((std::is_same<decltype(av_stdvec), ext::array_view<int>>::value));

    // (pointer, size) pair
    auto av_ptr = ext::make_array_view(stdvec.data(), 3);
    ASSERT_EQ(av_ptr.data(), stdvec.data());
    ASSERT_EQ(av_ptr.size(), 3);
    ASSERT_TRUE((std::is_same<decltype(av_ptr), ext::array_view<int>>::value));

    // Const array
    int const const_array[] = {12, 34, 56, 78, 90};
    auto av_const_array = ext::make_array_view(const_array);
    ASSERT_TRUE((std::is_same<decltype(av_const_array), ext::array_view<int const>>::value));

    // Const std::array
    auto const const_stdarray = std::array<int, 2> {{1234, 5678}};
    auto av_const_stdarray = ext::make_array_view(const_stdarray);
    ASSERT_TRUE((std::is_same<decltype(av_const_stdarray), ext::array_view<int const>>::value));

    // Const std::vector
    auto const const_stdvec = std::vector<int> {1234, 5678, 9012};
    auto av_const_stdvec = ext::make_array_view(const_stdvec);
    ASSERT_TRUE((std::is_same<decltype(av_const_stdvec), ext::array_view<int const>>::value));

    // (const ptr, size) pair
    auto av_const_ptr = ext::make_array_view(const_stdvec.data(), 2);
    ASSERT_TRUE((std::is_same<decltype(av_const_ptr), ext::array_view<int const>>::value));
}

TEST(ArrayView, Special)
{
    // Default construction
    ext::array_view<int> default_av;
    ASSERT_TRUE(default_av.empty());
    ASSERT_EQ(default_av.data(), nullptr);
    ASSERT_EQ(default_av.size(), 0);

    // Copy construction
    auto array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av = ext::make_array_view(array);
    auto av_copy = av;
    ASSERT_EQ(av_copy.data(), array.data());
    ASSERT_EQ(av_copy.size(), array.size());

    // Copy assignment
    auto vec = std::vector<int> {5, 6, 7, 8, 0};
    av_copy = ext::make_array_view(vec);
    ASSERT_EQ(av_copy.data(), vec.data());
    ASSERT_EQ(av_copy.size(), vec.size());

    // Swap
    auto v1 = std::vector<double> {1, 2, 3, 4};
    auto a2 = std::array<double, 5> {{5, 6, 7, 8, 9}};
    auto av1 = ext::make_array_view(v1);
    auto av2 = ext::make_array_view(a2);
    av1.swap(av2);
    ASSERT_EQ(av1.data(), a2.data());
    ASSERT_EQ(av1.size(), a2.size());
    ASSERT_EQ(av2.data(), v1.data());
    ASSERT_EQ(av2.size(), v1.size());
    swap(av1, av2);
    ASSERT_EQ(av1.data(), v1.data());
    ASSERT_EQ(av1.size(), v1.size());
    ASSERT_EQ(av2.data(), a2.data());
    ASSERT_EQ(av2.size(), a2.size());
}

TEST(ArrayView, RangeFor)
{
    // Range-based for
    auto array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av_array = ext::make_array_view(array);
    for (auto&& e : av_array)
        ASSERT_TRUE((std::is_same<decltype(e), int&>::value));

    // Const view of mutable array
    auto const& const_av_array = av_array;
    for (auto&& e : const_av_array)
        ASSERT_TRUE((std::is_same<decltype(e), int const&>::value));

    // View of const array
    auto const const_array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av_const_array = ext::make_array_view(const_array);
    for (auto&& e : av_const_array)
        ASSERT_TRUE((std::is_same<decltype(e), int const&>::value));
}

TEST(ArrayView, MutableElementAccess)
{
    auto array = std::array<int, 4> {{1234, 5678, 9012, 3456}};
    auto av = ext::make_array_view(array);

    // Read
    ASSERT_EQ(av.front(), 1234);
    ASSERT_EQ(av[1], 5678);
    ASSERT_EQ(av.at(2), 9012);
    ASSERT_EQ(av.back(), 3456);

    // Write
    av.front() = 0;
    ASSERT_EQ(array[0], 0);
    av[1] = 1;
    ASSERT_EQ(array[1], 1);
    av.at(2) = 2;
    ASSERT_EQ(array[2], 2);
    av.back() = 3;
    ASSERT_EQ(array[3], 3);
}

TEST(ArrayView, ConstElementAccess)
{
    // View of const array
    auto const const_array = std::array<int, 4> {{1234, 5678, 9012, 3456}};
    auto av_const = ext::make_array_view(const_array);
    ASSERT_TRUE((std::is_same<decltype(av_const.front()), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(av_const[1]), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(av_const.at(2)), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(av_const.back()), int const&>::value));
    ASSERT_EQ(av_const.front(), 1234);
    ASSERT_EQ(av_const[1], 5678);
    ASSERT_EQ(av_const.at(2), 9012);
    ASSERT_EQ(av_const.back(), 3456);

    // Const view of mutable array
    auto array = std::array<int, 4> {{5678, 9012, 3456, 7890}};
    auto const const_av = ext::make_array_view(array);
    ASSERT_TRUE((std::is_same<decltype(const_av.front()), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(const_av[1]), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(const_av.at(2)), int const&>::value));
    ASSERT_TRUE((std::is_same<decltype(const_av.back()), int const&>::value));
    ASSERT_EQ(const_av.front(), 5678);
    ASSERT_EQ(const_av[1], 9012);
    ASSERT_EQ(const_av.at(2), 3456);
    ASSERT_EQ(const_av.back(), 7890);
}

TEST(ArrayView, PopFrontPopBack)
{
    auto array = std::array<int, 5> {{12, 34, 56, 78, 90}};
    ext::array_view<int> av;

    // Single-element pop
    av = ext::make_array_view(array);
    av.pop_front();
    ASSERT_EQ(av.data(), array.data() + 1);
    ASSERT_EQ(av.size(), array.size() - 1);

    av = ext::make_array_view(array);
    av.pop_back();
    ASSERT_EQ(av.data(), array.data());
    ASSERT_EQ(av.size(), array.size() - 1);

    // Chunk pop
    av = ext::make_array_view(array);
    av.pop_front(3);
    ASSERT_EQ(av.data(), array.data() + 3);
    ASSERT_EQ(av.size(), array.size() - 3);

    av = ext::make_array_view(array);
    av.pop_back(3);
    ASSERT_EQ(av.data(), array.data());
    ASSERT_EQ(av.size(), array.size() - 3);
}

TEST(ArrayView, EqualityComparison)
{
    auto a1 = std::array<int, 5> {{12, 34, 56, 78, 90}};
    auto a2 = std::array<int, 5> {{12, 34, 56, 78, 90}};
    auto a3 = std::array<int, 5> {{12, 34, 56, 78,  9}};
    auto const av1 = ext::make_array_view(a1);
    auto const av2 = ext::make_array_view(a2);
    auto const av3 = ext::make_array_view(a3);
    ASSERT_EQ(av1, av2);
    ASSERT_NE(av1, av3);
    ASSERT_NE(av2, av3);

    ext::array_view<int> null_av;
    ASSERT_EQ(null_av, null_av);
    ASSERT_NE(null_av, av1);
}

TEST(ArrayView, LexicographicOrdering)
{
    auto a1 = std::array<int, 3> {{1, 2, 3}};
    auto a2 = std::array<int, 3> {{1, 2, 4}};
    auto a3 = std::array<int, 3> {{1, 3, 0}};
    auto const av1 = ext::make_array_view(a1);
    auto const av2 = ext::make_array_view(a2);
    auto const av3 = ext::make_array_view(a3);
    // <
    ASSERT_LT(av1, av2);
    ASSERT_LT(av2, av3);
    ASSERT_LT(av1, av3);
    // <=
    ASSERT_LE(av1, av2);
    ASSERT_LE(av2, av3);
    ASSERT_LE(av1, av3);
    ASSERT_LE(av1, av1); // x <= x
    ASSERT_LE(av2, av2); // x <= x
    ASSERT_LE(av3, av3); // x <= x
    // >
    ASSERT_GT(av2, av1);
    ASSERT_GT(av3, av2);
    ASSERT_GT(av3, av1);
    // >=
    ASSERT_GE(av2, av1);
    ASSERT_GE(av3, av2);
    ASSERT_GE(av3, av1);
    ASSERT_GE(av1, av1); // x >= x
    ASSERT_GE(av2, av2); // x >= x
    ASSERT_GE(av3, av3); // x >= x
}
