/*
 * Bounded view of raw memory region.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_ARRAY_VIEW_HPP
#define EXT_ARRAY_VIEW_HPP

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

#include <cassert>
#include <cstddef>

#include "contiguous_container.hpp"
#include "iterator_range.hpp"
#include "type_traits.hpp"

namespace ext
{
    /**
     * Non-owning view of contiguous memory region.
     */
    template<typename T>
    struct array_view
    {
        using access_type = T;
        using value_type = std::remove_const_t<T>;
        using pointer = access_type*;
        using reference = access_type&;
        using iterator = pointer;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        //----------------------------------------------------------------------
        // Constructors
        //----------------------------------------------------------------------

        /**
         * Creates an empty view.
         */
        constexpr
        array_view() = default;

        /**
         * Creates a view into specified memory region.
         *
         * The created view spans over the region starting from the element
         * pointed to by `begin` and ending at past the end element pointed to
         * by `end`. The view is empty if both `begin` and `end` are null or
         * the two pointers point to the same element.
         *
         * Behaviour is undefined if the two pointers are not null and do not
         * point to elements of the same array.
         */
        constexpr
        array_view(T* begin, T* end)
            : begin_ {begin}
            , end_ {end}
        {
            assert(begin_ <= end_);
        }

        /**
         * Creates a view into specified memory region.
         *
         * The created view spans over the contigous `size` elements starting
         * from the one pointed to by `data`. The view is empty if `size` is
         * zero.
         *
         * Behaviour is undefined if `data` is null and `size` is nonzero.
         */
        constexpr
        array_view(T* data, std::size_t size)
            : array_view {data, data + size}
        {
            assert(size == 0 || data != nullptr);
        }

        /**
         * Creates a view into contiguous container.
         */
        template<typename C,
                 std::enable_if_t<
                    ext::is_const_convertible<
                        ext::contiguous_access_type_t<C>, T>::value, int> = 0>
        constexpr
        array_view(C& data) noexcept
            : array_view {ext::begin_ptr(data), ext::end_ptr(data)}
        {
        }

        /**
         * Creates a view into array.
         */
        template<typename U,
                 std::size_t N,
                 std::enable_if_t<
                    ext::is_const_convertible<U, T>::value, int> = 0>
        constexpr
        array_view(U(& array)[N]) noexcept
            : array_view {array, array + N}
        {
        }

        /**
         * Converting constructor to support const covariance.
         */
        template<typename U,
                 std::enable_if_t<
                    ext::is_const_convertible<U, T>::value, int> = 0>
        constexpr
        array_view(array_view<U> other)
            : array_view {other.data(), other.size()}
        {
        }

        //----------------------------------------------------------------------
        // Range
        //----------------------------------------------------------------------

        /**
         * Returns an iterator to the beginning.
         */
        constexpr
        iterator begin() const noexcept
        {
            return begin_;
        }

        /**
         * Returns an iterator to the end.
         */
        constexpr
        iterator end() const noexcept
        {
            return end_;
        }

        //----------------------------------------------------------------------
        // Element access
        //----------------------------------------------------------------------

        /**
         * Accesses specified element.
         *
         * Returns a reference to the element at the specified index in the
         * viewed range. Constness of the view is not propagated to the element.
         *
         * Behaviour is undefined if `index >= size()`.
         */
        constexpr
        reference operator[](size_type index) const
        {
            return begin_[index];
        }

        /**
         * Accesses the first element.
         *
         * Returns a reference to the first element in the viewed range.
         * Constness of the view is not propagated to the element.
         *
         * Behaviour is undefined if the view is empty.
         */
        constexpr
        reference front() const
        {
            return *begin_;
        }

        /**
         * Accesses the last element.
         *
         * Returns a reference to the last element in the viewed range.
         * Constness of the view is not propagated to the element.
         *
         * Behaviour is undefined if the view is empty.
         */
        constexpr
        reference back() const
        {
            return *std::prev(end_);
        }

        /**
         * Returns a pointer to the first element.
         *
         * Constness of the view is not propagated to the pointer.
         */
        constexpr
        pointer data() const noexcept
        {
            return begin_;
        }

        //----------------------------------------------------------------------
        // Capacity
        //----------------------------------------------------------------------

        /**
         * Checks whether the view is empty.
         */
        constexpr
        bool empty() const noexcept
        {
            return begin_ == end_;
        }

        /**
         * Returns the number of elements.
         */
        constexpr
        size_type size() const noexcept
        {
            return static_cast<size_type>(end_ - begin_);
        }

        //----------------------------------------------------------------------
        // Modifiers
        //----------------------------------------------------------------------

        /**
         * Shrinks the view by moving its start forward.
         *
         * Nullary overload advances the start position by one. Unary overload
         * advances the start position by specified number.
         *
         * Behaviour is undefined if the start goes beyond the end.
         *
         * @returns `*this`
         */
        constexpr
        array_view& shrink_front()
        {
            ++begin_;
            assert(begin_ <= end_);
            return *this;
        }

        constexpr
        array_view& shrink_front(size_type n)
        {
            begin_ += n;
            assert(begin_ <= end_);
            return *this;
        }

        /**
         * Shrinks the view by moving its end backward.
         *
         * Nullary overload moves the end position by one backwards. Unary
         * overload moves the end position by specified number backwards.
         *
         * Behaviour is undefined if the end goes beyond the start.
         *
         * @returns `*this`
         */
        constexpr
        array_view& shrink_back()
        {
            --end_;
            assert(begin_ <= end_);
            return *this;
        }

        constexpr
        array_view& shrink_back(size_type n)
        {
            end_ -= n;
            assert(begin_ <= end_);
            return *this;
        }

        /**
         * Swaps the viewed region.
         */
        constexpr
        void swap(array_view& other) noexcept
        {
            std::swap(begin_, other.begin_);
            std::swap(end_, other.end_);
        }

        //----------------------------------------------------------------------
        // Querying operations
        //----------------------------------------------------------------------

        /**
         * Extracts a subarray.
         *
         * @param begin
         *      Inclusive index at which to begin extraction.
         *
         * @param end
         *      Non-inclusive index at which to end extraction.
         *
         * @returns
         *      A new array_view object viewing the extracted elements.
         */
        constexpr
        array_view slice(size_type begin) const
        {
            assert(begin <= size());
            return {begin_ + begin, end_};
        }

        constexpr
        array_view slice(size_type begin, size_type end) const
        {
            assert(begin <= end);
            assert(end <= size());
            return {begin_ + begin, begin_ + end};
        }

        /**
         * Returns a const view of the same memory region.
         */
        constexpr
        array_view<T const> const_view() const noexcept
        {
            return {begin_, end_};
        }

        //----------------------------------------------------------------------
        // Non-modifying algorithms
        //----------------------------------------------------------------------

        /**
         * Compares the contents with specified range.
         */
        template<typename I, typename S,
                 std::enable_if_t<ext::is_iterator_range<I, S>::value, int> = 0>
        bool equals(I first, S last) const
        {
            return std::equal(begin(), end(), first, last);
        }

        /**
         * Compares the contents with specified range.
         */
        template<typename R>
        bool equals(R const& range) const
        {
            using std::begin;
            using std::end;
            return equals(begin(range), end(range));
        }

        /**
         * Compares the contents with initializer list.
         */
        bool equals(std::initializer_list<T> il) const
        {
            return equals(il.begin(), il.end());
        }

        //----------------------------------------------------------------------
        // Modifying algorithms
        //----------------------------------------------------------------------

        /**
         * Assigns specified value to the elements.
         */
        void fill(value_type const& value) const
        {
            std::fill(begin(), end(), value);
        }

        /**
         * Assigns contents of specified range to the elements.
         */
        template<typename I, typename S,
                 std::enable_if_t<ext::is_iterator_range<I, S>::value, int> = 0>
        void assign(I first, S last) const
        {
            assert(static_cast<size_type>(std::distance(first, last))
                    == size());
            std::copy(first, last, begin());
        }

        /**
         * Assigns contents of specified range to the elements.
         */
        template<typename R>
        void assign(R const& range) const
        {
            using std::begin;
            using std::end;
            assign(begin(range), end(range));
        }

        /**
         * Assigns contents of initializer list to the elements.
         */
        void assign(std::initializer_list<T> il) const
        {
            assign(il.begin(), il.end());
        }

        //----------------------------------------------------------------------

      private:
        T* begin_ = nullptr;
        T* end_ = nullptr;
    };

    /**
     * Compares the viewed memory address (not content).
     */
    template<typename T>
    constexpr
    bool operator==(ext::array_view<T> a, ext::array_view<T> b) noexcept
    {
        return a.begin() == b.begin() && a.end() == b.end();
    }

    template<typename T>
    constexpr
    bool operator!=(ext::array_view<T> a, ext::array_view<T> b) noexcept
    {
        return !(a == b);
    }

    /**
     * Creates `ext::array_view` with element type deduced from argument.
     */
    template<typename T>
    constexpr
    ext::array_view<T> make_array_view(T* begin, T* end)
    {
        return {begin, end};
    }

    template<typename T>
    constexpr
    ext::array_view<T> make_array_view(T* data, std::size_t size)
    {
        return {data, size};
    }

    template<typename C, typename T = ext::contiguous_access_type_t<C>>
    constexpr
    ext::array_view<T> make_array_view(C& container) noexcept
    {
        return {container};
    }

    template<typename T, std::size_t N>
    constexpr
    ext::array_view<T> make_array_view(T(& array)[N]) noexcept
    {
        return {array};
    }
}

#endif
