#include <utility>
#include <catch.hpp>
#include <ext/any.hpp>


TEST_CASE("ext::any - default construction")
{
    ext::any any;
    CHECK_FALSE(any.has_value());
}

TEST_CASE("ext::any - initialize with int")
{
    ext::any any {42};
    CHECK(any.has_value());
    CHECK(any.has_value_of<int>());
    CHECK(any.value<int>() == 42);
}

TEST_CASE("ext::any - initialize with class")
{
    struct my_class
    {
        int id;
    };
    ext::any any {my_class {1234}};
    CHECK(any.has_value());
    CHECK(any.has_value_of<my_class>());
    CHECK_FALSE(any.has_value_of<int>());
    CHECK(any.value<my_class>().id == 1234);
}

TEST_CASE("ext::any - copy construction")
{
    ext::any any_1 {321};
    ext::any any_2 {any_1};
    CHECK(any_1.has_value());
    CHECK(any_2.has_value());
    CHECK(any_2.has_value_of<int>());
    CHECK(any_2.value<int>() == 321);
    CHECK(&any_1.value<int>() != &any_2.value<int>());
}

TEST_CASE("ext::any - copy assignment")
{
    ext::any any_1 {321};
    ext::any any_2;
    any_2 = any_1;
    CHECK(any_1.has_value());
    CHECK(any_2.has_value());
    CHECK(any_2.has_value_of<int>());
    CHECK(any_2.value<int>() == 321);
    CHECK(&any_1.value<int>() != &any_2.value<int>());
}

TEST_CASE("ext::any - move construction")
{
    ext::any any_1 {321};
    ext::any any_2 {std::move(any_1)};
    CHECK_FALSE(any_1.has_value());
    CHECK(any_2.has_value());
    CHECK(any_2.has_value_of<int>());
    CHECK(any_2.value<int>() == 321);
}

TEST_CASE("ext::any - move assignment")
{
    ext::any any_1 {321};
    ext::any any_2;
    any_2 = std::move(any_1);
    CHECK_FALSE(any_1.has_value());
    CHECK(any_2.has_value());
    CHECK(any_2.has_value_of<int>());
    CHECK(any_2.value<int>() == 321);
}

TEST_CASE("ext::any - implicit conversion")
{
    ext::any any;
    any = 42;
    CHECK(any.has_value_of<int>());
    CHECK(any.value<int>() == 42);
    any = 3.14;
    CHECK(any.has_value_of<double>());
    CHECK(any.value<double>() == 3.14);
}

TEST_CASE("ext::any - emplace")
{
    ext::any any;
    CHECK_FALSE(any.has_value());
    any.emplace<int>(1234);
    CHECK(any.has_value());
    CHECK(any.has_value_of<int>());
    CHECK(any.value<int>() == 1234);
}

TEST_CASE("ext::any - reset")
{
    ext::any any {123};
    CHECK(any.has_value());
    any.reset();
    CHECK_FALSE(any.has_value());
}

TEST_CASE("ext::any - swap")
{
    ext::any any_1 {123};
    ext::any any_2 {3.14};
    any_1.swap(any_2);
    CHECK(any_1.has_value_of<double>());
    CHECK(any_1.value<double>() == 3.14);
    CHECK(any_2.has_value_of<int>());
    CHECK(any_2.value<int>() == 123);
}

TEST_CASE("ext::any - modifying content")
{
    ext::any any = 42;
    any.value<int>() = 123;
    CHECK(any.value<int>() == 123);
}

TEST_CASE("ext::any - const correctness")
{
    ext::any const any = 42;
    CHECK(any.has_value());
    CHECK(any.has_value_of<int>());
    CHECK(any.value<int>() == 42);
    CHECK((std::is_same<decltype(any.value<int>()), int const&>::value));
}
