#include <memory>
#include <utility>

#include <catch.hpp>

#include <ext/clone_ptr.hpp>


namespace
{
    struct my_base
    {
        virtual
        ~my_base() = default;

        virtual
        int get_id() const = 0;

        virtual
        std::unique_ptr<my_base> clone() const = 0;
    };

    struct my_derived : my_base
    {
        int id;

        explicit
        my_derived(int id)
            : id {id}
        {
        }

        int get_id() const override
        {
            return id;
        }

        std::unique_ptr<my_base> clone() const override
        {
            return std::make_unique<my_derived>(*this);
        }
    };
}

TEST_CASE("ext::clone_ptr - default constructor")
{
    ext::clone_ptr<my_base> p;
    CHECK_FALSE(p);
}

TEST_CASE("ext::clone_ptr - nullptr")
{
    ext::clone_ptr<my_base> p {nullptr};
    CHECK_FALSE(p);
}

TEST_CASE("ext::clone_ptr - pointer constructor")
{
    ext::clone_ptr<my_base> p {new my_derived(0)};
    CHECK(p);
}

TEST_CASE("ext::clone_ptr - move construction")
{
    ext::clone_ptr<my_base> p {new my_derived(0)};
    ext::clone_ptr<my_base> q {std::move(p)};
    CHECK_FALSE(p);
    CHECK(q);
}

TEST_CASE("ext::clone_ptr - copy construction")
{
    ext::clone_ptr<my_base> p {new my_derived(0)};
    ext::clone_ptr<my_base> q {p};
    CHECK(p);
    CHECK(q);
    CHECK(p != q);
}

TEST_CASE("ext::clone_ptr - move assignment")
{
    ext::clone_ptr<my_base> p {new my_derived(0)};
    ext::clone_ptr<my_base> q;
    q = std::move(p);
    CHECK_FALSE(p);
    CHECK(q);
}

TEST_CASE("ext::clone_ptr - copy assignment")
{
    ext::clone_ptr<my_base> p {new my_derived(0)};
    ext::clone_ptr<my_base> q;
    q = p;
    CHECK(p);
    CHECK(q);
    CHECK(p != q);
}

TEST_CASE("ext::clone_ptr - dereference")
{
    ext::clone_ptr<my_base> p {new my_derived(42)};
    CHECK(p->get_id() == 42);
    CHECK((*p).get_id() == 42);
}

TEST_CASE("ext::clone_ptr - release")
{
    ext::clone_ptr<my_base> p {new my_derived(42)};
    std::unique_ptr<my_base> q {p.release()};
    CHECK_FALSE(p);
    CHECK(q->get_id() == 42);
}

TEST_CASE("ext::clone_ptr - reset to empty")
{
    ext::clone_ptr<my_base> p {new my_derived(42)};
    p.reset();
    CHECK_FALSE(p);
}

TEST_CASE("ext::clone_ptr - reset to null")
{
    ext::clone_ptr<my_base> p {new my_derived(42)};
    p.reset(nullptr);
    CHECK_FALSE(p);
}

TEST_CASE("ext::clone_ptr - reset to pointer")
{
    ext::clone_ptr<my_base> p {new my_derived(42)};
    p.reset(new my_derived(100));
    CHECK(p);
    CHECK(p->get_id() == 100);
}

TEST_CASE("ext::clone_ptr - swap")
{
    SECTION("swap valid pointers")
    {
        ext::clone_ptr<my_base> p {new my_derived(42)};
        ext::clone_ptr<my_base> q {new my_derived(100)};
        p.swap(q);
        CHECK(p);
        CHECK(q);
        CHECK(p->get_id() == 100);
        CHECK(q->get_id() == 42);
    }

    SECTION("one pointer is null")
    {
        ext::clone_ptr<my_base> p {new my_derived(42)};
        ext::clone_ptr<my_base> q;
        p.swap(q);
        CHECK_FALSE(p);
        CHECK(q);
        CHECK(q->get_id() == 42);
    }

    SECTION("both are null")
    {
        ext::clone_ptr<my_base> p;
        ext::clone_ptr<my_base> q;
        p.swap(q);
        CHECK_FALSE(p);
        CHECK_FALSE(q);
    }
}
