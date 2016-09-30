#include <type_traits>

#include <catch.hpp>

#include <ext/lifetime_utility.hpp>


TEST_CASE("ext::copy_counter - exception safety")
{
    CHECK((std::is_nothrow_copy_constructible<ext::copy_counter>::value));
    CHECK((std::is_nothrow_move_constructible<ext::copy_counter>::value));
    CHECK((std::is_nothrow_copy_assignable<ext::copy_counter>::value));
    CHECK((std::is_nothrow_move_assignable<ext::copy_counter>::value));
}

TEST_CASE("ext::copy_counter - default constructor")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);
}

TEST_CASE("ext::copy_counter - copy constructor")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);

    ext::copy_counter copy {original};
    CHECK(original.count() == 2);
    CHECK(copy.count() == 2);
}

TEST_CASE("ext::copy_counter - move constructor")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);

    ext::copy_counter moved_to {std::move(original)};
    CHECK(moved_to.count() == 1);
}

TEST_CASE("ext::copy_counter - copy assignment")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);

    ext::copy_counter copy;
    copy = original;
    CHECK(original.count() == 2);
    CHECK(copy.count() == 2);
}

TEST_CASE("ext::copy_counter - move assignment")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);

    ext::copy_counter moved_to;
    moved_to = std::move(original);
    CHECK(moved_to.count() == 1);
}

TEST_CASE("ext::copy_counter - destructor")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);
    {
        ext::copy_counter copy {original};
        CHECK(original.count() == 2);
        {
            ext::copy_counter grand_copy {original};
            CHECK(original.count() == 3);
            CHECK(copy.count() == 3);
            CHECK(grand_copy.count() == 3);
        }
        CHECK(copy.count() == 2);
        CHECK(original.count() == 2);
    }
    CHECK(original.count() == 1);
}

TEST_CASE("ext::copy_counter - move and copy")
{
    ext::copy_counter original;
    CHECK(original.count() == 1);

    ext::copy_counter a = original;
    CHECK(original.count() == 2);

    ext::copy_counter b = std::move(a);
    CHECK(original.count() == 2);
    {
        ext::copy_counter c = std::move(b);
        CHECK(original.count() == 2);

        ext::copy_counter d = c;
        CHECK(original.count() == 3);
    }
    CHECK(original.count() == 1);
}

TEST_CASE("ext::copy_counter - const")
{
    ext::copy_counter const const_original {};
    CHECK(const_original.count() == 1);

    ext::copy_counter const const_copy {const_original};
    CHECK(const_original.count() == 2);
    CHECK(const_copy.count() == 2);
}
