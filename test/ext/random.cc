#include <random>
#include <sstream>
#include <vector>
#include <cmath>

#include <catch.hpp>

#include <ext/random.hpp>


TEST_CASE("ext::xorshift1024_star / random engine interface")
{
    std::seed_seq seed_seq;

    // Constructors
    constexpr auto min = ext::xorshift1024_star::min();
    constexpr auto max = ext::xorshift1024_star::max();
    (void) min;
    (void) max;

    // Constructors
    auto engine = ext::xorshift1024_star();
    engine = ext::xorshift1024_star(0);
    engine = ext::xorshift1024_star(seed_seq);

    // Seeding functions
    engine.seed();
    engine.seed(0);
    engine.seed(seed_seq);

    // Generator functions
    auto value = ext::xorshift1024_star::result_type{engine()};
    (void) value;
    engine.discard(10uLL);

    // Comparison operators
    auto engine2 = engine;
    engine2.discard(10uLL);
    CHECK(engine == engine);
    CHECK(engine != engine2);
    CHECK(engine2 != engine);

    // Stream I/O
    std::ostringstream os;
    os << engine;
    std::istringstream is(os.str());
    is >> engine;
}

TEST_CASE("ext::jsf64 / random engine interface")
{
    std::seed_seq seed_seq;

    // Constructors
    constexpr auto min = ext::jsf64::min();
    constexpr auto max = ext::jsf64::max();
    (void) min;
    (void) max;

    // Constructors
    auto engine = ext::jsf64();
    engine = ext::jsf64(0);
    engine = ext::jsf64(seed_seq);

    // Seeding functions
    engine.seed();
    engine.seed(0);
    engine.seed(seed_seq);

    // Generator functions
    auto value = ext::jsf64::result_type{engine()};
    (void) value;
    engine.discard(10uLL);

    // Comparison operators
    auto engine2 = engine;
    engine2.discard(10uLL);
    CHECK(engine == engine);
    CHECK(engine != engine2);
    CHECK(engine2 != engine);

    // Stream I/O
    std::ostringstream os;
    os << engine;
    std::istringstream is(os.str());
    is >> engine;
}

TEST_CASE("ext::sfc64 / random engine interface")
{
    std::seed_seq seed_seq;

    // Constructors
    constexpr auto min = ext::sfc64::min();
    constexpr auto max = ext::sfc64::max();
    (void) min;
    (void) max;

    // Constructors
    auto engine = ext::sfc64();
    engine = ext::sfc64(0);
    engine = ext::sfc64(seed_seq);

    // Seeding functions
    engine.seed();
    engine.seed(0);
    engine.seed(seed_seq);

    // Generator functions
    auto value = ext::sfc64::result_type{engine()};
    (void) value;
    engine.discard(10uLL);

    // Comparison operators
    auto engine2 = engine;
    engine2.discard(10uLL);
    CHECK(engine == engine);
    CHECK(engine != engine2);
    CHECK(engine2 != engine);

    // Stream I/O
    std::ostringstream os;
    os << engine;
    std::istringstream is(os.str());
    is >> engine;
}

TEST_CASE("ext::generate_normal (float) / moment test", "[random]")
{
    auto estimate_moment = [](unsigned long sample_count, unsigned order)
        {
            auto engine = std::mt19937();
            auto sum = double(0);
            for (auto i = 0uL; i < sample_count; ++i)
                sum += std::pow(ext::generate_normal<float>(engine), order);
            return static_cast<float>(sum / sample_count);
        };

    // E[X^m] = (m/2)!! for even m; see https://oeis.org/A001147
    // V[X^m] = E[X^(2m)] - (E[X^m])^2
    auto const moments = std::vector<float> {
        1, 0, 1,  0,  3,   0,    15,      0,     105};
    auto const moment_variances = std::vector<float> {
        0, 1, 2, 15, 96, 945, 10170, 135135, 2016000};
    for (unsigned order = 1; order < moments.size(); ++order)
    {
        auto const sample_count = 1000000uL;
        auto const tolerance_2sigma = 2 * std::sqrt(moment_variances[order] / sample_count);
        auto const estimated_moment = estimate_moment(sample_count, order);
        CHECK(std::abs(estimated_moment - moments[order]) <= tolerance_2sigma);
    }
}

TEST_CASE("ext::generate_normal (double) / moment test", "[random]")
{
    auto estimate_moment = [](unsigned long sample_count, unsigned order)
        {
            auto engine = std::mt19937();
            auto sum = double(0);
            for (auto i = 0uL; i < sample_count; ++i)
                sum += std::pow(ext::generate_normal<double>(engine), order);
            return sum / sample_count;
        };

    // E[X^m] = (m/2)!! for even m; see https://oeis.org/A001147
    // V[X^m] = E[X^(2m)] - (E[X^m])^2
    auto const moments = std::vector<double> {
        1, 0, 1,  0,  3,   0,    15,      0,     105};
    auto const moment_variances = std::vector<double> {
        0, 1, 2, 15, 96, 945, 10170, 135135, 2016000};
    for (unsigned order = 1; order < moments.size(); ++order)
    {
        auto const sample_count = 1000000uL;
        auto const tolerance_2sigma = 2 * std::sqrt(moment_variances[order] / sample_count);
        auto const estimated_moment = estimate_moment(sample_count, order);
        CHECK(std::abs(estimated_moment - moments[order]) <= tolerance_2sigma);
    }
}
