#include <memory>
#include <type_traits>

#include <catch.hpp>

#include <ext/type_conversion.hpp>


TEST_CASE("ext::as_const")
{
    int original = 10;
    auto& result = ext::as_const(original);

    using type = decltype(result);
    CHECK((std::is_same<type, int const&>::value));

    CHECK(&result == &original);
}

TEST_CASE("ext::as_shared_ptr")
{
    int count = 0;

    struct counter
    {
        int& var;
        counter(int& var) : var {++var} {}
        ~counter() { --var; }
    };

    CHECK(count == 0);
    {
        std::unique_ptr<counter> p = std::make_unique<counter>(count);
        CHECK(count == 1);
        {
            std::shared_ptr<counter> sp = ext::as_shared_ptr(p.get());
            CHECK(count == 1);
        }
        CHECK(count == 1); // not destroyed by shared_ptr
    }
    CHECK(count == 0);
}
