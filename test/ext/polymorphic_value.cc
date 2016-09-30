#include <catch.hpp>

#include <ext/lifetime_utility.hpp>
#include <ext/polymorphic_value.hpp>

namespace
{
    struct my_base
    {
        virtual long id() const = 0;
        virtual long life() const = 0;

        bool operator==(my_base const& other) const
        {
            return id() == other.id();
        }
    };

    static_assert(!std::has_virtual_destructor<my_base>::value,
        "this class should not have virtual destructor to test if "
        "polymorphic_value invokes proper destructor");

    struct my_derived : my_base
    {
        my_derived(long id)
            : id_ {id}
        {
        }

        long id() const override
        {
            return id_;
        }

        long life() const override
        {
            return life_.count();
        }

      private:
        long id_;
        ext::copy_counter life_;
    };
}

TEST_CASE("ext::polymorphic_value - initialization")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    CHECK(v->id() == 123);
}

TEST_CASE("ext::polymorphic_value - copy constructor")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    ext::polymorphic_value<my_base> w {v};
    CHECK(&v.value() != &w.value());
    CHECK(w->id() == 123);
    CHECK(w->life() == 2); // two copies
}

TEST_CASE("ext::polymorphic_value - move constructor")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    ext::polymorphic_value<my_base> w {std::move(v)};
    CHECK(w->id() == 123);
    CHECK(w->life() == 1); // single object
}

TEST_CASE("ext::polymorphic_value - copy assignment")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    ext::polymorphic_value<my_base> w {my_derived {456}};
    w = v;
    CHECK(&v.value() != &w.value());
    CHECK(w->id() == 123);
    CHECK(w->life() == 2); // two copies
}

TEST_CASE("ext::polymorphic_value - move assignment")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    ext::polymorphic_value<my_base> w {my_derived {456}};
    w = std::move(v);
    CHECK(w->id() == 123);
    CHECK(w->life() == 1); // single object
}

TEST_CASE("ext::polymorphic_value - destructor")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    {
        ext::polymorphic_value<my_base> w {v};
        CHECK(w->id() == 123);
        CHECK(w->life() == 2);
        CHECK(v->life() == 2);
    }
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - assignment of new value")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    v = my_derived {456};
    CHECK(v->id() == 456);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - emplace")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    v.emplace<my_derived>(456);
    CHECK(v->id() == 456);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - swap")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    ext::polymorphic_value<my_base> w {my_derived {456}};
    v.swap(w);
    CHECK(v->id() == 456);
    CHECK(v->life() == 1);
    CHECK(w->id() == 123);
    CHECK(w->life() == 1);
}

TEST_CASE("ext::polymorphic_value - value (mutable)")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};

    CHECK((std::is_same<decltype(v.value()), my_base&>::value));
    my_base& base = v.value();
    CHECK(base.id() == 123);
    CHECK(base.life() == 1);

    // Sanity check
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - value (const)")
{
    ext::polymorphic_value<my_base> const v {my_derived {123}};

    CHECK((std::is_same<decltype(v.value()), my_base const&>::value));
    my_base const& base = v.value();
    CHECK(base.id() == 123);
    CHECK(base.life() == 1);

    // Sanity check
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - as (mutable)")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};

    using type = decltype(v.as<my_derived>());
    CHECK((std::is_same<type, my_derived&>::value));
    my_derived& derived = v.as<my_derived>();
    CHECK(derived.id() == 123);
    CHECK(derived.life() == 1);

    // Sanity check
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - as (const)")
{
    ext::polymorphic_value<my_base> const v {my_derived {123}};

    using type = decltype(v.as<my_derived>());
    CHECK((std::is_same<type, my_derived const&>::value));
    my_derived const& derived = v.as<my_derived>();
    CHECK(derived.id() == 123);
    CHECK(derived.life() == 1);

    // Sanity check
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - member access")
{
    ext::polymorphic_value<my_base> v {my_derived {123}};
    CHECK(v->id() == 123);
    CHECK(v->life() == 1);
}

TEST_CASE("ext::polymorphic_value - equality comparison")
{
    ext::polymorphic_value<my_base> const a {my_derived {123}};
    ext::polymorphic_value<my_base> const b {my_derived {123}};
    ext::polymorphic_value<my_base> const c {my_derived {456}};

    CHECK(a == a);
    CHECK(a == b);
    CHECK(a != c);

    CHECK(b == a);
    CHECK(b == b);
    CHECK(b != c);

    CHECK(c != a);
    CHECK(c != b);
    CHECK(c == c);
}
