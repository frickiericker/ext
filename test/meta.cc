#include <gtest/gtest.h>

#include <ext/meta.hpp>


namespace MakeNonDeduced
{
    // Without make_non_deduced the template argument cannot be deduced when,
    // for example, the first function argument is a value of type double and
    // the second function argument is a value of type int.
    template<typename T> void test_1(T, typename ext::make_non_deduced<T>::type) {}
    template<typename T> void test_2(T, ext::make_non_deduced_t<T>) {}
}

TEST(MakeNonDeduced, Test)
{
    MakeNonDeduced::test_1(1.0, 1);
    MakeNonDeduced::test_2(1.0, 1);
}
