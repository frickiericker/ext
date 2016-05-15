#include <gtest/gtest.h>

#include <memory>
#include <ext/smart_pointer.hpp>

//----------------------------------------------------------------------------
// view_as_shared
//----------------------------------------------------------------------------

TEST(ViewAsShared, ViewLocal)
{
    int var = 42;
    {
        std::shared_ptr<int> ptr = ext::view_as_shared(&var);
        ASSERT_EQ(ptr.get(), &var);
        *ptr = 10;
    }
    ASSERT_EQ(var, 10);
}

//----------------------------------------------------------------------------
// value_ptr
//----------------------------------------------------------------------------

TEST(ValuePtr, Null)
{
    ext::value_ptr<int> null1;
    ASSERT_FALSE(null1);
    ASSERT_EQ(null1.get(), nullptr);

    ext::value_ptr<int> null2{nullptr};
    ASSERT_FALSE(null2);
    ASSERT_EQ(null2.get(), nullptr);
}

TEST(ValuePtr, Copy)
{
    ext::value_ptr<int> val{42};
    ext::value_ptr<int> copy{val};
    ASSERT_TRUE(val);
    ASSERT_TRUE(copy);
    ASSERT_EQ(*copy, 42);
    *copy = 10;
    ASSERT_EQ(*copy, 10);
    ASSERT_EQ(*val, 42);
}

TEST(ValuePtr, Move)
{
    ext::value_ptr<int> val{42};
    ext::value_ptr<int> move{std::move(val)};
    ASSERT_FALSE(val);
    ASSERT_TRUE(move);
    ASSERT_EQ(*move, 42);
}

TEST(ValuePtr, Polymorphism)
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
    ASSERT_TRUE(foo);
    ASSERT_EQ(foo->value(), 42);

    // Copy without slicing
    ext::value_ptr<base> foo_copy{foo};
    ASSERT_EQ(foo_copy->value(), 42);

    // Assign new value without slicing
    foo = ext::value_ptr<base>{derived{10}};
    ASSERT_EQ(foo->value(), 10);

    // Move without slicing
    ext::value_ptr<base> foo_move{std::move(foo)};
    ASSERT_EQ(foo_move->value(), 10);
}
