#include <memory>

#include <catch.hpp>

#include <ext/smart_pointer.hpp>

//----------------------------------------------------------------------------
// view_as_shared
//----------------------------------------------------------------------------

TEST_CASE("ext::view_as_shared / pointer to local object")
{
    int var = 42;
    {
        std::shared_ptr<int> ptr = ext::view_as_shared(&var);
        CHECK(ptr.get() == &var);
        *ptr = 10;
    }
    CHECK(var == 10);
}

//----------------------------------------------------------------------------
// value_ptr
//----------------------------------------------------------------------------

TEST_CASE("ext::value_ptr / null")
{
    ext::value_ptr<int> null1;
    CHECK_FALSE(null1);
    CHECK(null1.get() == nullptr);

    ext::value_ptr<int> null2{nullptr};
    CHECK_FALSE(null2);
    CHECK(null2.get() == nullptr);
}

TEST_CASE("ext::value_ptr / copying")
{
    ext::value_ptr<int> val{42};
    ext::value_ptr<int> copy{val};
    CHECK(val);
    CHECK(copy);
    CHECK(*copy == 42);
    *copy = 10;
    CHECK(*copy == 10);
    CHECK(*val == 42);
}

TEST_CASE("ext::value_ptr / moving")
{
    ext::value_ptr<int> val{42};
    ext::value_ptr<int> move{std::move(val)};
    CHECK_FALSE(val);
    CHECK(move);
    CHECK(*move == 42);
}

TEST_CASE("ext::value_ptr / polymorphism")
{
    struct base
    {
        virtual ~base() = default;
        virtual int value() = 0;
    };
    struct derived : base
    {
        int value_;
        explicit derived(int val) : value_{val} {}
        int value() override { return value_; }
    };

    ext::value_ptr<base> foo{derived{42}};
    CHECK(foo);
    CHECK(foo->value() == 42);

    // Copy without slicing
    ext::value_ptr<base> foo_copy{foo};
    CHECK(foo_copy->value() == 42);

    // Assign new value without slicing
    foo = ext::value_ptr<base>{derived{10}};
    CHECK(foo->value() == 10);

    // Move without slicing
    ext::value_ptr<base> foo_move{std::move(foo)};
    CHECK(foo_move->value() == 10);
}
