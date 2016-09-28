#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <ext/iterator_range.hpp>


namespace
{
    struct cstring_sentinel
    {
    };

    bool operator==(char const* p, cstring_sentinel)
    {
        return *p == '\0';
    }

    bool operator==(cstring_sentinel s, char const* p)
    {
        return p == s;
    }

    bool operator!=(char const* p, cstring_sentinel s)
    {
        return !(p == s);
    }

    bool operator!=(cstring_sentinel s, char const* p)
    {
        return !(s == p);
    }
}

TEST_CASE("cstring_sentinel")
{
    char const* p_beg = "a";
    char const* p_end = p_beg + 1;
    cstring_sentinel s;
    REQUIRE(p_end == s);
    REQUIRE(s == p_end);
    REQUIRE(p_beg != s);
    REQUIRE(s != p_beg);
}

TEST_CASE("ext::is_iterator_range")
{
    SECTION("pointer pair")
    {
        CHECK((ext::is_iterator_range<int*, int*>::value));
    }

    SECTION("vector iterator pair")
    {
        using iterator = std::vector<int>::iterator;
        CHECK((ext::is_iterator_range<iterator, iterator>::value));
    }

    SECTION("iterator and sentinel")
    {
        CHECK((ext::is_iterator_range<char const*, cstring_sentinel>::value));
    }

    SECTION("false examples")
    {
        // Mismatch
        CHECK_FALSE((ext::is_iterator_range<int*, double*>::value));

        // Obvious non-iterator
        CHECK_FALSE((ext::is_iterator_range<int, int>::value));
        CHECK_FALSE((ext::is_iterator_range<void, void>::value));

        // Completely unrelated
        CHECK_FALSE((ext::is_iterator_range<void, char[5]>::value));
    }
}

TEST_CASE("ext::iterator_range - construction")
{
    std::vector<int> vec {1, 2, 3};

    ext::iterator_range<std::vector<int>::iterator> r {vec.begin(), vec.end()};
    CHECK(r.begin() == vec.begin());
    CHECK(r.end() == vec.end());

    for (int& element : r)
    {
        int* ptr = &element;
        CHECK(ptr >= vec.data());
        CHECK(ptr < vec.data() + vec.size());
    }
}

TEST_CASE("ext::iterator_range - factory")
{
    std::vector<int> vec {1, 2, 3, 4, 5};

    auto r = ext::range(vec.begin() + 1, vec.end() - 1);

    using type = std::decay_t<decltype(r)>;
    using expected = ext::iterator_range<std::vector<int>::iterator>;
    CHECK((std::is_same<type, expected>::value));

    CHECK(r.begin() == vec.begin() + 1);
    CHECK(r.end() == vec.end() - 1);
}
