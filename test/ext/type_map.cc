#include <unordered_set>

#include <cstddef>

#include <catch.hpp>

#include <ext/type_map.hpp>


TEST_CASE("ext::type_directory")
{
    // Number of referred types are determined in program startup
    CHECK(ext::type_directory::size() >= 5);

    // Here five types are queried
    std::unordered_set<std::size_t> const indices {
        ext::type_directory::index<int>,
        ext::type_directory::index<char>,
        ext::type_directory::index<char const>,
        ext::type_directory::index<struct some_type>,
        ext::type_directory::index<struct other_type>
    };
    CHECK(indices.size() == std::size_t(5));
}

TEST_CASE("ext::type_map - example usage")
{
    ext::type_map<int> map;

    map.associate<char>(1);
    map.associate<short>(2);
    map.associate<int>(4);

    CHECK(map.value<char>() == 1);
    CHECK(map.value<short>() == 2);
    CHECK(map.value<int>() == 4);

    map.value<char>() = 100;
    map.value<short>() = 200;
    map.value<int>() = 400;
    CHECK(map.value<char>() == 100);
    CHECK(map.value<short>() == 200);
    CHECK(map.value<int>() == 400);
}
