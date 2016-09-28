#include <type_traits>

#include <catch.hpp>

#include <ext/type_traits.hpp>


TEST_CASE("ext::identity")
{
    CHECK((std::is_same<ext::identity_t<int>,       int      >::value));
    CHECK((std::is_same<ext::identity_t<int&>,      int&     >::value));
    CHECK((std::is_same<ext::identity_t<int&&>,     int&&    >::value));
    CHECK((std::is_same<ext::identity_t<int const>, int const>::value));
}

TEST_CASE("ext::is_const_convertible")
{
    // May add const
    CHECK((      ext::is_const_convertible<int,       int      >::value));
    CHECK((      ext::is_const_convertible<int,       int const>::value));
    CHECK_FALSE((ext::is_const_convertible<int const, int      >::value));
    CHECK((      ext::is_const_convertible<int const, int const>::value));

    // No upcasting
    struct base {};
    struct derived : base {};

    CHECK(      (ext::is_const_convertible<derived,       derived      >::value));
    CHECK(      (ext::is_const_convertible<derived,       derived const>::value));
    CHECK_FALSE((ext::is_const_convertible<derived const, derived      >::value));
    CHECK(      (ext::is_const_convertible<derived const, derived const>::value));

    CHECK_FALSE((ext::is_const_convertible<derived,       base      >::value));
    CHECK_FALSE((ext::is_const_convertible<derived,       base const>::value));
    CHECK_FALSE((ext::is_const_convertible<derived const, base const>::value));
    CHECK_FALSE((ext::is_const_convertible<derived const, base      >::value));

    // Unrelated types
    CHECK_FALSE((ext::is_const_convertible<int,       char      >::value));
    CHECK_FALSE((ext::is_const_convertible<int,       char const>::value));
    CHECK_FALSE((ext::is_const_convertible<int const, char      >::value));
    CHECK_FALSE((ext::is_const_convertible<int const, char const>::value));

    // No casting to void
    CHECK_FALSE((ext::is_const_convertible<int,       void      >::value));
    CHECK_FALSE((ext::is_const_convertible<int,       void const>::value));
    CHECK_FALSE((ext::is_const_convertible<int const, void      >::value));
    CHECK_FALSE((ext::is_const_convertible<int const, void const>::value));
}
