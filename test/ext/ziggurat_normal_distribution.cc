#include <random>
#include <vector>

#include <cmath>

#include <catch.hpp>

#include <ext/ziggurat_normal_distribution.hpp>


TEST_CASE("ext::ziggurat_normal_distribution - moment test", "[random]")
{
    std::mt19937 engine;
    ext::ziggurat_normal_distribution<double> normal;

    auto estimate_moment = [&](unsigned long sample_count, unsigned order)
        {
            double sum = 0;
            for (auto i = 0uL; i < sample_count; ++i) {
                sum += std::pow(normal(engine), order);
            }
            return sum / double(sample_count);
        };

    // E[X^m] = (m/2)!!  for even m
    // E[X^m] = 0        for odd m
    std::vector<double> const moments {
        1, 0, 1, 0, 3, 0, 15, 0, 105
    };

    // V[X^m] = E[X^(2m)] - (E[X^m])^2
    std::vector<double> const variances {
        0, 1, 2, 15, 96, 945, 10170, 135135, 2016000
    };

    for (unsigned order = 1; order < moments.size(); ++order)
    {
        auto const sample_count = 1000000uL;
        auto const tolerance = 2 * std::sqrt(variances[order] / sample_count);
        auto const estimated_moment = estimate_moment(sample_count, order);
        CHECK(estimated_moment == Approx(moments[order]).epsilon(tolerance));
    }
}
