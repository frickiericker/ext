/*
 * Normal distribution implemented with ziggurat algorithm.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_ZIGGURAT_NORMAL_DISTRIBUTION_HPP
#define EXT_ZIGGURAT_NORMAL_DISTRIBUTION_HPP

#include <random>
#include <utility>

#include <cassert>
#include <cmath>
#include <cstddef>

namespace ext
{
    template<typename Tag, typename Dummy = void>
    struct ziggurat_tables;

    template<std::size_t BlockCount>
    struct normal_distribution_tag;

    template<typename D>
    struct ziggurat_tables<normal_distribution_tag<128>, D>
    {
        static constexpr std::size_t block_count = 128;
        static constexpr double tail_start = 3.442619855899;
        static double const xs[block_count + 1];
        static double const ys[block_count];
    };

    namespace detail
    {
        inline
        double normal_weight(double x)
        {
            return std::exp(-x * x / 2);
        }

        template<std::size_t bits, typename URNG>
        std::pair<unsigned, double> generate_bits_and_canonical(URNG& engine)
        {
            static constexpr auto rest_width = ((URNG::max() - URNG::min()) >> bits) + 1u;
            static constexpr auto mask = (1u << bits) - 1u;
            static constexpr auto norm = 1.0 / rest_width;
            auto const value = engine() - URNG::min();
            return std::pair<unsigned, double>(value & mask, (value >> bits) * norm);
        }
    }

    /**
     * Produces real values on the normal distribution.
     *
     * This implementation uses ziggurat algorithm [1] to generate normally
     * distributed numbers. The algorithm uses a lookup table and is very fast
     * compared to the conventional Box-Muller transform method.
     *
     * [1]: https://en.wikipedia.org/wiki/Ziggurat_algorithm
     */
    template<typename T, std::size_t BlockCount = 128>
    struct ziggurat_normal_distribution
    {
        static_assert(BlockCount == 128, "Currently impelemented only for 128 blocks");

        using result_type = T;

      private:
        static constexpr std::size_t block_count = 128;
        static constexpr std::size_t canonical_bits = 48; // FIXME
        using ziggurat = ziggurat_tables<normal_distribution_tag<block_count>>;

      public:
        result_type min() const
        {
            return -std::numeric_limits<result_type>::infinity;
        }

        result_type max() const
        {
            return std::numeric_limits<result_type>::infinity;
        }

        template<typename URNG>
        result_type operator()(URNG& engine)
        {
            return static_cast<result_type>(sample(engine));
        }

      private:
        template<typename URNG>
        double sample(URNG& engine) const
        {
            for (;;)
            {
                auto const r = detail::generate_bits_and_canonical<8>(engine);
                auto const layer = std::get<0>(r) & 0x7F;
                auto const sign = (std::get<0>(r) & 0x80) ? 1.0 : -1.0;
                auto const u = std::get<1>(r);
                auto const x = u * ziggurat::xs[layer];

                if (x < ziggurat::xs[layer + 1]) // taken 98% of times
                    return sign * x;

                if (layer == 0)
                    return sign * sample_from_tail(engine);

                // Rejection sampling out of the interval [xs[layer+1], xs[layer]).
                auto const y = ziggurat::ys[layer - 1]
                                + (ziggurat::ys[layer] - ziggurat::ys[layer - 1])
                                    * std::generate_canonical<double, canonical_bits>(engine);
                if (y < detail::normal_weight(x))
                    return sign * x;
            }
        }

        template<typename URNG>
        double sample_from_tail(URNG& engine) const
        {
            double x, y;
            do
            {
                auto const s = std::generate_canonical<double, canonical_bits>(engine);
                auto const t = std::generate_canonical<double, canonical_bits>(engine);
                x = -std::log(s) / ziggurat::tail_start;
                y = -std::log(t);
            }
            while (2 * y < x * x);

            return ziggurat::tail_start + x;
        }
    };

    template<typename T, std::size_t BlockCount>
    bool operator==(ziggurat_normal_distribution<T, BlockCount> const&,
                    ziggurat_normal_distribution<T, BlockCount> const&)
    {
        return true;
    }

    template<typename T, std::size_t BlockCount>
    bool operator!=(ziggurat_normal_distribution<T, BlockCount> const&,
                    ziggurat_normal_distribution<T, BlockCount> const&)
    {
        return false;
    }
}

template<typename D>
double const ext::ziggurat_tables<ext::normal_distribution_tag<128>, D>::xs[] =
{
    3.71308624674255, 3.44261985589900, 3.22308498458114, 3.08322885821687,
    2.97869625264778, 2.89434400702153, 2.82312535054891, 2.76116937238718,
    2.70611357312182, 2.65640641126136, 2.61097224843185, 2.56903362592494,
    2.53000967238883, 2.49345452209537, 2.45901817741183, 2.42642064553375,
    2.39543427801106, 2.36587137011764, 2.33757524133924, 2.31041368369876,
    2.28427405967747, 2.25905957386920, 2.23468639559098, 2.21108140887870,
    2.18818043207605, 2.16592679374892, 2.14427018236039, 2.12316570867398,
    2.10257313518924, 2.08245623799202, 2.06278227450831, 2.04352153665507,
    2.02464697337738, 2.00613386996347, 1.98795957412762, 1.97010326085433,
    1.95254572955356, 1.93526922829662, 1.91825730086451, 1.90149465310515,
    1.88496703570776, 1.86866114099449, 1.85256451172809, 1.83666546025844,
    1.82095299659612, 1.80541676421923, 1.79004698259986, 1.77483439558607,
    1.75977022489959, 1.74484612811380, 1.73005416056373, 1.71538674071367,
    1.70083661856992, 1.68639684677917, 1.67206075409760, 1.65782192095402,
    1.64367415686287, 1.62961147947063, 1.61562809504316, 1.60171838022138,
    1.58787686489057, 1.57409821602300, 1.56037722236617, 1.54670877985991,
    1.53308787767404, 1.51950958476594, 1.50596903686320, 1.49246142378135,
    1.47898197698992, 1.46552595734271, 1.45208864288922, 1.43866531668456,
    1.42525125451406, 1.41184171244706, 1.39843191413100, 1.38501703773265,
    1.37159220242734, 1.35815245433014, 1.34469275175355, 1.33120794966563,
    1.31769278320941, 1.30414185012862, 1.29054959192619, 1.27691027356015,
    1.26321796145462, 1.24946649957307, 1.23564948326336, 1.22176023053999,
    1.20779175041595, 1.19373670783313, 1.17958738466399, 1.16533563616475,
    1.15097284214887, 1.13648985201316, 1.12187692258254, 1.10712364753403,
    1.09221887690728, 1.07715062489289, 1.06190596369482, 1.04647090076404,
    1.03083023606819, 1.01496739525133, 0.99886423349298, 0.98250080351543,
    0.96585507940115, 0.94890262551130, 0.93161619661515, 0.91396525102303,
    0.89591535258093, 0.87742742911292, 0.85845684319381, 0.83895221429757,
    0.81885390670035, 0.79809206064405, 0.77658398789476, 0.75423066445405,
    0.73091191064249, 0.70647961133543, 0.68074791866915, 0.65347863873997,
    0.62435859733605, 0.59296294247144, 0.55869217840818, 0.52065603876205,
    0.47743783729668, 0.42654798635542, 0.36287143109702, 0.27232086481395,
    0.0
};

template<typename D>
double const ext::ziggurat_tables<ext::normal_distribution_tag<128>, D>::ys[] =
{
    0.00266962908388, 0.00554899522077, 0.00862448441286, 0.01183947865788,
    0.01516729801055, 0.01859210273701, 0.02210330461593, 0.02569329193593,
    0.02935631744001, 0.03308788614623, 0.03688438878666, 0.04074286807444,
    0.04466086220049, 0.04863629585987, 0.05266740190305, 0.05675266348105,
    0.06089077034804, 0.06508058521307, 0.06932111739358, 0.07361150188411,
    0.07795098251397, 0.08233889824224, 0.08677467189478, 0.09125780082683,
    0.09578784912173, 0.10036444102866, 0.10498725540942, 0.10965602101484,
    0.11437051244887, 0.11913054670765, 0.12393598020287, 0.12878670619594,
    0.13368265258344, 0.13862377998459, 0.14361008009063, 0.14864157424234,
    0.15371831220818, 0.15884037113948, 0.16400785468342, 0.16922089223737,
    0.17447963833079, 0.17978427212330, 0.18513499700899, 0.19053204031914,
    0.19597565311628, 0.20146611007431, 0.20700370943993, 0.21258877307173,
    0.21822164655431, 0.22390269938501, 0.22963232523212, 0.23541094226348,
    0.24123899354544, 0.24711694751232, 0.25304529850733, 0.25902456739621,
    0.26505530225559, 0.27113807913839, 0.27727350291919, 0.28346220822323,
    0.28970486044296, 0.29600215684693, 0.30235482778648, 0.30876363800618,
    0.31522938806501, 0.32175291587599, 0.32833509837285, 0.33497685331359,
    0.34167914123155, 0.34844296754633, 0.35526938484792, 0.36215949536932,
    0.36911445366447, 0.37613546951056, 0.38322381105590, 0.39038080823732,
    0.39760785649387, 0.40490642080722, 0.41227804010266, 0.41972433204958,
    0.42724699830500, 0.43484783024999, 0.44252871527547, 0.45029164368204,
    0.45813871626787, 0.46607215268946, 0.47409430069302, 0.48220764632949,
    0.49041482528385, 0.49871863547098, 0.50712205107557, 0.51562823824400,
    0.52424057267299, 0.53296265938384, 0.54179835502543, 0.55075179311461,
    0.55982741270409, 0.56902999106795, 0.57836468111976, 0.58783705443471,
    0.59745315094452, 0.60721953662512, 0.61714337081888, 0.62723248524993,
    0.63749547733504, 0.64794182111022, 0.65858200005009, 0.66942766734889,
    0.68049184099734, 0.69178914343668, 0.70333609901616, 0.71515150741050,
    0.72725691834419, 0.73967724367265, 0.75244155917461, 0.76558417389771,
    0.77914608592969, 0.79317701177131, 0.80773829468296, 0.82290721138141,
    0.83878360529599, 0.85550060786945, 0.87324304891007, 0.89228165078403,
    0.91304364797174, 0.93628268168506, 0.96359969312709, 0.99999999995644
};

#endif
