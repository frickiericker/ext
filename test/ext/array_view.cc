#include <array>
#include <stdexcept>
#include <vector>
#include <cstddef>

#include <catch.hpp>

#include <ext/array_view.hpp>


TEST_CASE("ext::array_view / construction")
{
    // Array
    int array[] = {1234, 5678};
    auto av_array = ext::make_array_view(array);
    CHECK(av_array.data() == array);
    CHECK(av_array.size() == std::size_t(2));
    CHECK((std::is_same<decltype(av_array), ext::array_view<int>>::value));

    // std::array
    auto stdarray = std::array<int, 3> {{1234, 5678, 9012}};
    auto av_stdarray = ext::make_array_view(stdarray);
    CHECK(av_stdarray.data() == stdarray.data());
    CHECK(av_stdarray.size() == stdarray.size());
    CHECK((std::is_same<decltype(av_stdarray), ext::array_view<int>>::value));

    // std::vector
    auto stdvec = std::vector<int> {12, 34, 56, 78, 90};
    auto av_stdvec = ext::make_array_view(stdvec);
    CHECK(av_stdvec.data() == stdvec.data());
    CHECK(av_stdvec.size() == stdvec.size());
    CHECK((std::is_same<decltype(av_stdvec), ext::array_view<int>>::value));

    // (pointer, size) pair
    auto av_ptr = ext::make_array_view(stdvec.data(), 3);
    CHECK(av_ptr.data() == stdvec.data());
    CHECK(av_ptr.size() == std::size_t(3));
    CHECK((std::is_same<decltype(av_ptr), ext::array_view<int>>::value));

    // Const array
    int const const_array[] = {12, 34, 56, 78, 90};
    auto av_const_array = ext::make_array_view(const_array);
    CHECK((std::is_same<decltype(av_const_array), ext::array_view<int const>>::value));

    // Const std::array
    auto const const_stdarray = std::array<int, 2> {{1234, 5678}};
    auto av_const_stdarray = ext::make_array_view(const_stdarray);
    CHECK((std::is_same<decltype(av_const_stdarray), ext::array_view<int const>>::value));

    // Const std::vector
    auto const const_stdvec = std::vector<int> {1234, 5678, 9012};
    auto av_const_stdvec = ext::make_array_view(const_stdvec);
    CHECK((std::is_same<decltype(av_const_stdvec), ext::array_view<int const>>::value));

    // (const ptr, size) pair
    auto av_const_ptr = ext::make_array_view(const_stdvec.data(), 2);
    CHECK((std::is_same<decltype(av_const_ptr), ext::array_view<int const>>::value));
}

TEST_CASE("ext::array_view / special functions")
{
    // Default construction
    ext::array_view<int> default_av;
    CHECK(default_av.empty());
    CHECK(default_av.data() == nullptr);
    CHECK(default_av.size() == std::size_t(0));

    // Copy construction
    auto array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av = ext::make_array_view(array);
    auto av_copy = av;
    CHECK(av_copy.data() == array.data());
    CHECK(av_copy.size() == array.size());

    // Copy assignment
    auto vec = std::vector<int> {5, 6, 7, 8, 0};
    av_copy = ext::make_array_view(vec);
    CHECK(av_copy.data() == vec.data());
    CHECK(av_copy.size() == vec.size());

    // Swap
    auto v1 = std::vector<double> {1, 2, 3, 4};
    auto a2 = std::array<double, 5> {{5, 6, 7, 8, 9}};
    auto av1 = ext::make_array_view(v1);
    auto av2 = ext::make_array_view(a2);
    av1.swap(av2);
    CHECK(av1.data() == a2.data());
    CHECK(av1.size() == a2.size());
    CHECK(av2.data() == v1.data());
    CHECK(av2.size() == v1.size());
    swap(av1, av2);
    CHECK(av1.data() == v1.data());
    CHECK(av1.size() == v1.size());
    CHECK(av2.data() == a2.data());
    CHECK(av2.size() == a2.size());
}

TEST_CASE("ext::array_view / range-based for")
{
    // Range-based for
    auto array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av_array = ext::make_array_view(array);
    for (auto&& e : av_array)
        CHECK((std::is_same<decltype(e), int&>::value));

    // Const view of mutable array
    auto const& const_av_array = av_array;
    for (auto&& e : const_av_array)
        CHECK((std::is_same<decltype(e), int const&>::value));

    // View of const array
    auto const const_array = std::array<int, 4> {{1, 2, 3, 4}};
    auto av_const_array = ext::make_array_view(const_array);
    for (auto&& e : av_const_array)
        CHECK((std::is_same<decltype(e), int const&>::value));
}

TEST_CASE("ext::array_view / mutable element access")
{
    auto array = std::array<int, 4> {{1234, 5678, 9012, 3456}};
    auto av = ext::make_array_view(array);

    // Read
    CHECK(av.front() == 1234);
    CHECK(av[1] == 5678);
    CHECK(av.at(2) == 9012);
    CHECK(av.back() == 3456);

    // Write
    av.front() = 0;
    CHECK(array[0] == 0);
    av[1] = 1;
    CHECK(array[1] == 1);
    av.at(2) = 2;
    CHECK(array[2] == 2);
    av.back() = 3;
    CHECK(array[3] == 3);
}

TEST_CASE("ext::array_view / const element access")
{
    // View of const array
    auto const const_array = std::array<int, 4> {{1234, 5678, 9012, 3456}};
    auto av_const = ext::make_array_view(const_array);
    CHECK((std::is_same<decltype(av_const.front()), int const&>::value));
    CHECK((std::is_same<decltype(av_const[1]), int const&>::value));
    CHECK((std::is_same<decltype(av_const.at(2)), int const&>::value));
    CHECK((std::is_same<decltype(av_const.back()), int const&>::value));
    CHECK(av_const.front() == 1234);
    CHECK(av_const[1] == 5678);
    CHECK(av_const.at(2) == 9012);
    CHECK(av_const.back() == 3456);

    // Const view of mutable array
    auto array = std::array<int, 4> {{5678, 9012, 3456, 7890}};
    auto const const_av = ext::make_array_view(array);
    CHECK((std::is_same<decltype(const_av.front()), int const&>::value));
    CHECK((std::is_same<decltype(const_av[1]), int const&>::value));
    CHECK((std::is_same<decltype(const_av.at(2)), int const&>::value));
    CHECK((std::is_same<decltype(const_av.back()), int const&>::value));
    CHECK(const_av.front() == 5678);
    CHECK(const_av[1] == 9012);
    CHECK(const_av.at(2) == 3456);
    CHECK(const_av.back() == 7890);
}

TEST_CASE("ext::array_view / pop_front/back")
{
    auto array = std::array<int, 5> {{12, 34, 56, 78, 90}};
    ext::array_view<int> av;

    // Single-element pop
    av = ext::make_array_view(array);
    av.pop_front();
    CHECK(av.data() == array.data() + 1);
    CHECK(av.size() == array.size() - 1);

    av = ext::make_array_view(array);
    av.pop_back();
    CHECK(av.data() == array.data());
    CHECK(av.size() == array.size() - 1);

    // Chunk pop
    av = ext::make_array_view(array);
    av.pop_front(3);
    CHECK(av.data() == array.data() + 3);
    CHECK(av.size() == array.size() - 3);

    av = ext::make_array_view(array);
    av.pop_back(3);
    CHECK(av.data() == array.data());
    CHECK(av.size() == array.size() - 3);
}

TEST_CASE("ext::array_view / equality comparison")
{
    auto a1 = std::array<int, 5> {{12, 34, 56, 78, 90}};
    auto a2 = std::array<int, 5> {{12, 34, 56, 78, 90}};
    auto a3 = std::array<int, 5> {{12, 34, 56, 78,  9}};
    auto const av1 = ext::make_array_view(a1);
    auto const av2 = ext::make_array_view(a2);
    auto const av3 = ext::make_array_view(a3);
    CHECK(av1 == av2);
    CHECK(av1 != av3);
    CHECK(av2 != av3);

    ext::array_view<int> null_av;
    CHECK(null_av == null_av);
    CHECK(null_av != av1);
}

TEST_CASE("ext::array_view / lexicographic ordering")
{
    auto a1 = std::array<int, 3> {{1, 2, 3}};
    auto a2 = std::array<int, 3> {{1, 2, 4}};
    auto a3 = std::array<int, 3> {{1, 3, 0}};
    auto const av1 = ext::make_array_view(a1);
    auto const av2 = ext::make_array_view(a2);
    auto const av3 = ext::make_array_view(a3);

    CHECK(av1 < av2);
    CHECK(av2 < av3);
    CHECK(av1 < av3);

    CHECK(av1 <= av2);
    CHECK(av2 <= av3);
    CHECK(av1 <= av3);
    CHECK(av1 <= av1); // x <= x
    CHECK(av2 <= av2); // x <= x
    CHECK(av3 <= av3); // x <= x

    CHECK(av2 > av1);
    CHECK(av3 > av2);
    CHECK(av3 > av1);

    CHECK(av2 >= av1);
    CHECK(av3 >= av2);
    CHECK(av3 >= av1);
    CHECK(av1 >= av1); // x >= x
    CHECK(av2 >= av2); // x >= x
    CHECK(av3 >= av3); // x >= x
}
