#include <array>
#include <deque>
#include <vector>

#include <catch.hpp>

#include <ext/contiguous_container.hpp>


TEST_CASE("ext::has_contiguous_access")
{
    // Standard containers
    CHECK((ext::has_contiguous_access<std::array<int, 5>>::value));
    CHECK((ext::has_contiguous_access<std::vector<int>>::value));
    CHECK_FALSE((ext::has_contiguous_access<std::deque<int>>::value));

    // Obvious ones
    CHECK_FALSE((ext::has_contiguous_access<void>::value));
    CHECK_FALSE((ext::has_contiguous_access<int>::value));

    // Arrays are not because they does not have member functions
    CHECK_FALSE((ext::has_contiguous_access<int[5]>::value));

    // Programmer-defined class
    struct my_class {};
    struct my_undefined_class;
    CHECK_FALSE((ext::has_contiguous_access<my_class>::value));
    CHECK_FALSE((ext::has_contiguous_access<my_undefined_class>::value));
}

TEST_CASE("ext::contiguous_access_type")
{
    SECTION("mutable vector")
    {
        using result = ext::contiguous_access_type_t<std::vector<int>>;
        using expected = int;
        CHECK((std::is_same<result, expected>::value));
    }

    SECTION("const vector")
    {
        using result = ext::contiguous_access_type_t<std::vector<int> const>;
        using expected = int const;
        CHECK((std::is_same<result, expected>::value));
    }
}

TEST_CASE("ext::begin_ptr/end_ptr")
{
    SECTION("vector")
    {
        std::vector<int> vec {1, 2, 3};
        int* beg = ext::begin_ptr(vec);
        int* end = ext::end_ptr(vec);
        CHECK(beg == vec.data());
        CHECK(end == vec.data() + vec.size());
    }

    SECTION("const vector")
    {
        std::vector<int> const vec {1, 2, 3};
        int const* beg = ext::begin_ptr(vec);
        int const* end = ext::end_ptr(vec);
        CHECK(beg == vec.data());
        CHECK(end == vec.data() + vec.size());
    }

    SECTION("builtin array")
    {
        int arr[5] {1, 2, 3, 4, 5};
        int* beg = ext::begin_ptr(arr);
        int* end = ext::end_ptr(arr);
        CHECK(beg == arr);
        CHECK(end == arr + 5);
    }
}
