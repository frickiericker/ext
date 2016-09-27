/*
 * Handling objects iterable with range-based for.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_ITERATOR_RANGE_HPP
#define EXT_ITERATOR_RANGE_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace ext
{
    /**
     * Checks if two types form a range.
     */
    template<typename I, typename S>
    struct is_iterator_range;

    namespace detail
    {
        template<typename I, typename S,
                 typename Tr = std::iterator_traits<I>,
                 typename = typename Tr::value_type,
                 typename = typename Tr::reference,
                 typename = typename Tr::pointer,
                 typename = typename Tr::difference_type,
                 typename = typename Tr::iterator_category,
                 typename = decltype(std::declval<I>() == std::declval<S>())>
        std::true_type probe_iterator_range(int);

        template<typename I, typename S>
        std::false_type probe_iterator_range(...);
    }

    template<typename I, typename S>
    struct is_iterator_range : decltype(detail::probe_iterator_range<I, S>(0))
    {
    };

    /**
     * Lightweight object holding an iterator and sentinel.
     */
    template<typename I, typename S = I>
    struct iterator_range
    {
        using iterator = I;
        using sentinel = S;

        /**
         * Range spanning over specified start and end.
         */
        iterator_range(iterator first, sentinel last)
            : first_ {first}, last_ {last}
        {
        }

        /**
         * Returns the beginning of the range.
         */
        constexpr
        iterator begin() const noexcept
        {
            return first_;
        }

        /**
         * Returns the ending of the range.
         */
        constexpr
        sentinel end() const noexcept
        {
            return last_;
        }

      private:
        I first_;
        S last_;
    };

    /**
     * Creates a `ext::iterator_range` object.
     */
    template<typename I, typename S>
    ext::iterator_range<I, S> range(I first, S last)
    {
        return {first, last};
    }
}

#endif
