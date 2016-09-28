#include <array>
#include <type_traits>
#include <vector>
#include <cstddef>
#include <catch.hpp>
#include <ext/array_view.hpp>


TEST_CASE("ext::array_view - default construction")
{
    ext::array_view<int> av;
    CHECK(av.empty());
    CHECK(av.size() == std::size_t(0));
    CHECK(av.data() == nullptr);
    CHECK(av.begin() == av.end());
}

TEST_CASE("ext::array_view - address range constructor")
{
    int arr[5] {1, 2, 3, 4, 5};

    SECTION("non-empty")
    {
        ext::array_view<int> av {arr, arr + 5};
        CHECK_FALSE(av.empty());
        CHECK(av.size() == std::size_t(5));
        CHECK(av.data() == arr);
    }

    SECTION("empty")
    {
        ext::array_view<int> av {arr, arr};
        CHECK(av.empty());
        CHECK(av.size() == std::size_t(0));
        CHECK(av.data() == arr);
    }
}

TEST_CASE("ext::array_view - data/size constructor")
{
    int arr[5] {1, 2, 3, 4, 5};

    SECTION("non-empty")
    {
        ext::array_view<int> av {arr, 5};
        CHECK_FALSE(av.empty());
        CHECK(av.size() == std::size_t(5));
        CHECK(av.data() == arr);
    }

    SECTION("empty")
    {
        ext::array_view<int> av {arr, std::size_t(0)};
        CHECK(av.empty());
        CHECK(av.size() == std::size_t(0));
        CHECK(av.data() == arr);
    }
}

TEST_CASE("ext::array_view - contiguous container constructor")
{
    std::vector<int> vec {1, 2, 3, 4, 5};

    SECTION("direct initialization")
    {
        ext::array_view<int> av {vec};
        CHECK_FALSE(av.empty());
        CHECK(av.size() == vec.size());
        CHECK(av.data() == vec.data());
    }

    SECTION("implicit conversion")
    {
        ext::array_view<int> av = vec;
        CHECK_FALSE(av.empty());
        CHECK(av.size() == vec.size());
        CHECK(av.data() == vec.data());
    }

    SECTION("empty")
    {
        std::vector<int> empty_vec;
        ext::array_view<int> av = empty_vec;
        CHECK(av.empty());
        CHECK(av.size() == std::size_t(0));
    }
}

TEST_CASE("ext::array_view - array constructor")
{
    int arr[5] {1, 2, 3, 4, 5};

    SECTION("direct initialization")
    {
        ext::array_view<int> av {arr};
        CHECK_FALSE(av.empty());
        CHECK(av.size() == std::size_t(5));
        CHECK(av.data() == arr);
    }

    SECTION("implicit conversion")
    {
        ext::array_view<int> av = arr;
        CHECK_FALSE(av.empty());
        CHECK(av.size() == std::size_t(5));
        CHECK(av.data() == arr);
    }
}

TEST_CASE("ext::array_view - const covariance")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av_mutable {arr};
    ext::array_view<int const> av_const = av_mutable;
    CHECK(av_const.data() == av_mutable.data());
    CHECK(av_const.size() == av_mutable.size());
}

TEST_CASE("ext::array_view - copy constructor")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av {arr};
    ext::array_view<int> av_copy = av;
    CHECK(av_copy.data() == av.data());
    CHECK(av_copy.size() == av.size());
}

TEST_CASE("ext::array_view - element access")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av = arr;

    SECTION("check values")
    {
        CHECK(av.front() == 1);
        CHECK(av.back() == 5);
        CHECK(av[2] == 3);
    }

    SECTION("modify elements")
    {
        av.front() = 100;
        av.back() = 500;
        av[2] = 300;
        CHECK(arr[0] == 100);
        CHECK(arr[1] == 2);
        CHECK(arr[2] == 300);
        CHECK(arr[3] == 4);
        CHECK(arr[4] == 500);
    }
}

TEST_CASE("ext::array_view - element access via const view")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> const av = arr;

    SECTION("check values")
    {
        CHECK(av.front() == 1);
        CHECK(av.back() == 5);
        CHECK(av[2] == 3);
    }

    SECTION("modify elements") // no const propagation
    {
        av.front() = 100;
        av.back() = 500;
        av[2] = 300;
        CHECK(arr[0] == 100);
        CHECK(arr[1] == 2);
        CHECK(arr[2] == 300);
        CHECK(arr[3] == 4);
        CHECK(arr[4] == 500);
    }
}

TEST_CASE("ext::array_view - range access")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av = arr;

    auto first = av.begin();
    auto last = av.end();
    CHECK(last - first == 5);
    CHECK(*first == 1);
}

TEST_CASE("ext::array_view - swap")
{
    int arr_a[5] {1, 2, 3, 4, 5};
    int arr_b[3] {6, 7, 8};
    ext::array_view<int> a = arr_a;
    ext::array_view<int> b = arr_b;
    a.swap(b);
    CHECK(a.data() == arr_b);
    CHECK(a.size() == std::size_t(3));
    CHECK(b.data() == arr_a);
    CHECK(b.size() == std::size_t(5));
}

TEST_CASE("ext::array_view - shrink")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av = arr;

    SECTION("shrink_front by one")
    {
        av.shrink_front();
        CHECK(av.front() == 2);
        CHECK(av.back() == 5);
        CHECK(av.size() == 4);
    }

    SECTION("shrink_front by some amount")
    {
        av.shrink_front(3);
        CHECK(av.front() == 4);
        CHECK(av.back() == 5);
        CHECK(av.size() == 2);
    }

    SECTION("shrink_back by one")
    {
        av.shrink_back();
        CHECK(av.front() == 1);
        CHECK(av.back() == 4);
        CHECK(av.size() == 4);
    }

    SECTION("shrink_back by one")
    {
        av.shrink_back(3);
        CHECK(av.front() == 1);
        CHECK(av.back() == 2);
        CHECK(av.size() == 2);
    }

    SECTION("method chain")
    {
        auto x = av.shrink_front(1).shrink_back(2).front();
        CHECK(x == 2);
        CHECK(av.front() == 2);
        CHECK(av.back() == 3);
        CHECK(av.size() == 2);
    }
}

TEST_CASE("ext::array_view - slice")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av = arr;

    SECTION("specify only beginning")
    {
        ext::array_view<int> av_slice = av.slice(2);
        CHECK(av_slice.front() == 3);
        CHECK(av_slice.back() == 5);
        CHECK(av_slice.size() == 3);
    }

    SECTION("specify both ends")
    {
        ext::array_view<int> av_slice = av.slice(1, 3);
        CHECK(av_slice.front() == 2);
        CHECK(av_slice.back() == 3);
        CHECK(av_slice.size() == 2);
    }

    SECTION("empty slice")
    {
        ext::array_view<int> av_slice = av.slice(2, 2);
        CHECK(av_slice.empty());
        CHECK(av_slice.data() == av.data() + 2);
    }
}

TEST_CASE("ext::array_view - const_view")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> av = arr;

    using result_type = std::decay_t<decltype(av.const_view())>;
    using expected_type = ext::array_view<int const>;
    CHECK((std::is_same<result_type, expected_type>::value));

    ext::array_view<int const> av_const = av.const_view();
    CHECK(av_const.data() == av.data());
    CHECK(av_const.size() == av.size());
}

TEST_CASE("ext::array_view - equals algorithm")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> const av = arr;

    SECTION("against iterator range")
    {
        std::array<int, 5> match {{1, 2, 3, 4, 5}};
        std::array<int, 5> unmatch {{1, 2, 3, 2, 1}};
        CHECK(av.equals(match.begin(), match.end()));
        CHECK_FALSE(av.equals(unmatch.begin(), unmatch.end()));
    }

    SECTION("against iterator range / different length")
    {
        std::array<int, 3> shorter {{1, 2, 3}};
        std::array<int, 7> longer {{1, 2, 3, 4, 5, 6, 7}};
        CHECK_FALSE(av.equals(shorter.begin(), shorter.end()));
        CHECK_FALSE(av.equals(longer.begin(), longer.end()));

        // Compare against empty iterator range
        CHECK_FALSE(av.equals(shorter.begin(), shorter.begin()));
    }

    SECTION("against range")
    {
        std::array<int, 5> match {{1, 2, 3, 4, 5}};
        std::array<int, 5> unmatch {{1, 2, 3, 2, 1}};
        CHECK(av.equals(match));
        CHECK_FALSE(av.equals(unmatch));
    }

    SECTION("against range / different length")
    {
        std::array<int, 3> shorter {{1, 2, 3}};
        std::array<int, 7> longer {{1, 2, 3, 4, 5, 6, 7}};
        CHECK_FALSE(av.equals(shorter));
        CHECK_FALSE(av.equals(longer));

        std::vector<int> empty;
        CHECK_FALSE(av.equals(empty));
    }

    SECTION("against initializer_list")
    {
        CHECK(av.equals({1, 2, 3, 4, 5}));
        CHECK_FALSE(av.equals({1, 2, 3, 2, 1}));
        CHECK_FALSE(av.equals({1, 2, 3}));
        CHECK_FALSE(av.equals({1, 2, 3, 4, 5, 6, 7}));
        CHECK_FALSE(av.equals({}));
    }

    SECTION("with slicing")
    {
        CHECK(av.slice(1, 4).equals({2, 3, 4}));
    }

    SECTION("view is empty")
    {
        ext::array_view<int> const av_empty = av.slice(0, 0);

        std::array<int, 5> unmatch {{1, 2, 3, 4, 5}};
        CHECK_FALSE(av_empty.equals(unmatch.begin(), unmatch.end()));
        CHECK_FALSE(av_empty.equals(unmatch));
        CHECK_FALSE(av_empty.equals({1, 2, 3}));

        std::vector<int> empty;
        CHECK(av_empty.equals(empty.begin(), empty.end()));
        CHECK(av_empty.equals(empty));
        CHECK(av_empty.equals({}));
    }
}

TEST_CASE("ext::array_view - fill algorithm")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> const av = arr;

    SECTION("just fill")
    {
        av.fill(9);
        CHECK(arr[0] == 9);
        CHECK(arr[1] == 9);
        CHECK(arr[2] == 9);
        CHECK(arr[3] == 9);
        CHECK(arr[4] == 9);

        av.fill(0);
        CHECK(arr[0] == 0);
        CHECK(arr[1] == 0);
        CHECK(arr[2] == 0);
        CHECK(arr[3] == 0);
        CHECK(arr[4] == 0);
    }

    SECTION("with slicing")
    {
        av.slice(1, 3).fill(0);
        CHECK(arr[0] == 1);
        CHECK(arr[1] == 0);
        CHECK(arr[2] == 0);
        CHECK(arr[3] == 4);
        CHECK(arr[4] == 5);
    }

    SECTION("empty view")
    {
        av.slice(0, 0).fill(0);
        CHECK(arr[0] == 1);
        CHECK(arr[1] == 2);
        CHECK(arr[2] == 3);
        CHECK(arr[3] == 4);
        CHECK(arr[4] == 5);
    }
}

TEST_CASE("ext::array_view - assign algorithm")
{
    int arr[5] {1, 2, 3, 4, 5};
    ext::array_view<int> const av = arr;

    SECTION("from iterator range")
    {
        std::array<int, 5> values {{11, 12, 13, 14, 15}};
        av.assign(values.begin(), values.end());
        CHECK(arr[0] == 11);
        CHECK(arr[1] == 12);
        CHECK(arr[2] == 13);
        CHECK(arr[3] == 14);
        CHECK(arr[4] == 15);
    }

    SECTION("from range")
    {
        std::array<int, 5> values {{11, 12, 13, 14, 15}};
        av.assign(values);
        CHECK(arr[0] == 11);
        CHECK(arr[1] == 12);
        CHECK(arr[2] == 13);
        CHECK(arr[3] == 14);
        CHECK(arr[4] == 15);
    }

    SECTION("from initializer_list")
    {
        av.assign({11, 12, 13, 14, 15});
        CHECK(arr[0] == 11);
        CHECK(arr[1] == 12);
        CHECK(arr[2] == 13);
        CHECK(arr[3] == 14);
        CHECK(arr[4] == 15);
    }

    SECTION("with slicing")
    {
        av.slice(1, 4).assign({9, 8, 7});
        CHECK(arr[0] == 1);
        CHECK(arr[1] == 9);
        CHECK(arr[2] == 8);
        CHECK(arr[3] == 7);
        CHECK(arr[4] == 5);
    }

    SECTION("view is empty")
    {
        ext::array_view<int> const av_empty = av.slice(0, 0);

        std::vector<int> empty;
        av_empty.assign(empty.begin(), empty.end());
        av_empty.assign(empty);
        av_empty.assign({});
        CHECK(arr[0] == 1);
        CHECK(arr[1] == 2);
        CHECK(arr[2] == 3);
        CHECK(arr[3] == 4);
        CHECK(arr[4] == 5);
    }
}

TEST_CASE("ext::array_view - factory functions")
{
    SECTION("pointer pair")
    {
        std::array<int, 5> const arr {{1, 2, 3, 4, 5}};
        auto result = ext::make_array_view(arr.data(), arr.data() + arr.size());

        using result_type = std::decay_t<decltype(result)>;
        using expected_type = ext::array_view<int const>;
        CHECK((std::is_same<result_type, expected_type>::value));

        CHECK(result.data() == arr.data());
        CHECK(result.size() == arr.size());
    }

    SECTION("pointer and size")
    {
        std::array<int, 5> const arr {{1, 2, 3, 4, 5}};
        auto result = ext::make_array_view(arr.data(), arr.size());

        using result_type = std::decay_t<decltype(result)>;
        using expected_type = ext::array_view<int const>;
        CHECK((std::is_same<result_type, expected_type>::value));

        CHECK(result.data() == arr.data());
        CHECK(result.size() == arr.size());
    }

    SECTION("contiguous container")
    {
        std::array<int, 5> const arr {{1, 2, 3, 4, 5}};
        auto result = ext::make_array_view(arr);

        using result_type = std::decay_t<decltype(result)>;
        using expected_type = ext::array_view<int const>;
        CHECK((std::is_same<result_type, expected_type>::value));

        CHECK(result.data() == arr.data());
        CHECK(result.size() == arr.size());
    }

    SECTION("array")
    {
        int const arr[5] {1, 2, 3, 4, 5};
        auto result = ext::make_array_view(arr);

        using result_type = std::decay_t<decltype(result)>;
        using expected_type = ext::array_view<int const>;
        CHECK((std::is_same<result_type, expected_type>::value));

        CHECK(result.data() == arr);
        CHECK(result.size() == std::size_t(5));
    }
}
