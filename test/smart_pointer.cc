#include <gtest/gtest.h>

#include <memory>
#include <ext/smart_pointer.hpp>

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
