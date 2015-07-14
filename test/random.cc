#include <random>
#include <vector>
#include <cmath>

#include <gtest/gtest.h>

#include <ext/random.hpp>


TEST(GenerateNormal, MomentTestFloat)
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
        ASSERT_LE(std::abs(estimated_moment - moments[order]), tolerance_2sigma);
    }
}

TEST(GenerateNormal, MomentTestDouble)
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
        ASSERT_LE(std::abs(estimated_moment - moments[order]), tolerance_2sigma);
    }
}
