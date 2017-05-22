#include <catch.hpp>

#include <ext/algorithm.hpp>


TEST_CASE("ext::iota_n - basic use")
{
    int array[5] = {-1, -1, -1, -1, -1};
    int* end = ext::iota_n(array, 4, 3);
    CHECK(end == array + 4);
    CHECK(array[0] == 3);
    CHECK(array[1] == 4);
    CHECK(array[2] == 5);
    CHECK(array[3] == 6);
    CHECK(array[4] == -1);
}

TEST_CASE("ext::iota_n - empty range")
{
    int array[3] = {-1, -1, -1};
    int* end = ext::iota_n(array, 0, 0);
    CHECK(end == array);
    CHECK(array[0] == -1);
    CHECK(array[1] == -1);
    CHECK(array[2] == -1);
}
