#ifndef EXT_ITERATOR_HPP
#define EXT_ITERATOR_HPP

#include <iterator>
#include <type_traits>

#include <cstddef>

#include <boost/range/adaptor/strided.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/iterator_range.hpp>


namespace ext
{
    template<typename T>
    auto size(T const& c) noexcept(noexcept(c.size())) -> decltype(c.size())
    {
        return c.size();
    }

    template<typename T, std::size_t N>
    constexpr
    std::size_t size(T(&)[N]) noexcept
    {
        return N;
    }
}

namespace ext
{
    namespace detail
    {
        // Poor-man's Iterator concept
        template< typename T
                , typename = typename std::iterator_traits<T>::iterator_category>
        auto detect_iterator(int)
        -> std::true_type;

        template<typename T>
        auto detect_iterator(...)
        -> std::false_type;

        template<typename T>
        struct is_iterator : decltype(detect_iterator<T>(0))
        {
        };
    }
}

namespace ext
{
    template< typename Integer
            , std::enable_if_t<std::is_integral<Integer>::value, int> = 0>
    decltype(auto) range(Integer last)
    {
        return boost::irange(Integer(), last);
    }

    template< typename Integer
            , std::enable_if_t<std::is_integral<Integer>::value, int> = 0>
    decltype(auto) range(Integer first, Integer last)
    {
        return boost::irange(first, last);
    }

    template< typename Integer
            , std::enable_if_t<std::is_integral<Integer>::value, int> = 0>
    decltype(auto) range(Integer first, Integer last, Integer step)
    {
        return boost::irange(first, last, step);
    }

    template< typename Iterator
            , std::enable_if_t<detail::is_iterator<Iterator>::value, int> = 0>
    decltype(auto) range(Iterator first, Iterator last)
    {
        return boost::make_iterator_range(first, last);
    }

    template< typename Iterator
            , std::enable_if_t<detail::is_iterator<Iterator>::value, int> = 0>
    decltype(auto) range(Iterator first, Iterator last,
                         typename std::iterator_traits<Iterator>::difference_type step)
    {
        return boost::adaptors::stride(boost::make_iterator_range(first, last), step);
    }
}

#endif
