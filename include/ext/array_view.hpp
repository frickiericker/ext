#ifndef EXT_ARRAY_VIEW_HPP
#define EXT_ARRAY_VIEW_HPP

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <cassert>
#include <cstddef>


namespace ext
{
    /**
     * Non-owning view of a sequential memory region.
     */
    template<typename T>
    struct array_view
    {
        using access_type = T;
        using value_type = std::remove_const_t<T>;
        using pointer = access_type*;
        using const_pointer = access_type const*;
        using reference = access_type&;
        using const_reference = access_type const&;
        using iterator = access_type*;
        using const_iterator = access_type const*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        //__ Creation ______________________________________________________

        /**
         * Constructs a null view.
         */
        constexpr
        array_view() noexcept
            : ptr_(nullptr), size_(0)
        {
        }

        /**
         * Constructs a view into the array holded by a std::vector.
         */
        template<typename Allocator>
        explicit
        array_view(std::vector<value_type, Allocator>& vec) noexcept
            : ptr_(vec.data()), size_(vec.size())
        {
        }

        template<typename Allocator>
        explicit
        array_view(std::vector<value_type, Allocator> const& vec) noexcept
            : ptr_(vec.data()), size_(vec.size())
        {
        }

        /**
         * Constructs a view into a std::array.
         */
        template<size_type N>
        explicit constexpr
        array_view(std::array<value_type, N>& ary) noexcept
            : ptr_(ary.data()), size_(ary.size())
        {
        }

        template<size_type N>
        explicit constexpr
        array_view(std::array<value_type, N> const& ary) noexcept
            : ptr_(ary.data()), size_(ary.size())
        {
        }

        /**
         * Constructs a view into a built-in array.
         */
        template<size_type N>
        explicit constexpr
        array_view(access_type(& ary)[N]) noexcept
            : ptr_(ary), size_(N)
        {
        }

        /**
         * Constructs a view into a memory region.
         */
        constexpr
        array_view(pointer ptr, size_type count)
            : ptr_(ptr), size_(count)
        {
        }

        //__ Range Support _________________________________________________

        constexpr iterator begin() noexcept { return ptr_; }
        constexpr iterator end() noexcept { return ptr_ + size_; }
        constexpr const_iterator begin() const noexcept { return ptr_; }
        constexpr const_iterator end() const noexcept { return ptr_ + size_; }
        constexpr const_iterator cbegin() const noexcept { return ptr_; }
        constexpr const_iterator cend() const noexcept { return ptr_ + size_; }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        reverse_iterator rend() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rend() const noexcept { return reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return reverse_iterator(begin()); }

        //__ Element Access Functions ______________________________________

        constexpr
        reference operator[](size_type idx)
        {
            return ptr_[idx];
        }

        constexpr
        const_reference operator[](size_type idx) const
        {
            return ptr_[idx];
        }

        constexpr
        reference at(size_type idx)
        {
            if (idx > size_)
                throw std::range_error("out of range");
            return ptr_[idx];
        }

        constexpr
        const_reference at(size_type idx) const
        {
            if (idx > size_)
                throw std::range_error("out of range");
            return ptr_[idx];
        }

        constexpr
        reference front()
        {
            return *ptr_;
        }

        constexpr
        const_reference front() const
        {
            return *ptr_;
        }

        constexpr
        reference back()
        {
            return ptr_[size_ - 1];
        }

        constexpr
        const_reference back() const
        {
            return ptr_[size_ - 1];
        }

        constexpr
        pointer data() noexcept
        {
            return ptr_;
        }

        constexpr
        const_pointer data() const noexcept
        {
            return ptr_;
        }

        //__ Capacity Query ________________________________________________

        constexpr
        size_type size() const noexcept
        {
            return size_;
        }

        constexpr
        bool empty() const noexcept
        {
            return size() == 0;
        }

        //__ Range Modification ____________________________________________

        constexpr
        void pop_front()
        {
            ++ptr_;
            --size_;
        }

        constexpr
        void pop_back()
        {
            --size_;
        }

        constexpr
        void pop_front(size_type n)
        {
            ptr_ += n;
            size_ -= n;
        }

        constexpr
        void pop_back(size_type n)
        {
            size_ -= n;
        }

        constexpr
        void swap(array_view& other) noexcept
        {
            std::swap(ptr_, other.ptr_);
            std::swap(size_, other.size_);
        }

        constexpr friend
        void swap(array_view& a, array_view& b) noexcept
        {
            a.swap(b);
        }

        //__________________________________________________________________

      private:

        pointer ptr_;
        size_type size_;
    };

    //__ Comparison Operators ______________________________________________

    template<typename T>
    bool operator==(array_view<T> const& a, array_view<T> const& b)
    {
        return a.size() == b.size() && std::equal(a.cbegin(), a.cend(), b.cbegin(), b.cend());
    }

    template<typename T>
    bool operator!=(array_view<T> const& a, array_view<T> const& b)
    {
        return !(a == b);
    }

    template<typename T>
    bool operator<(array_view<T> const& a, array_view<T> const& b)
    {
        return std::lexicographical_compare(a.cbegin(), a.cend(), b.cbegin(), b.cend());
    }

    template<typename T>
    bool operator>(array_view<T> const& a, array_view<T> const& b)
    {
        return b < a;
    }

    template<typename T>
    bool operator<=(array_view<T> const& a, array_view<T> const& b)
    {
        return !(a > b);
    }

    template<typename T>
    bool operator>=(array_view<T> const& a, array_view<T> const& b)
    {
        return !(a < b);
    }

    //__ Factory Functions _________________________________________________

    /**
     * Creates an array_view into the storage of a std::vector.
     */
    template<typename T, typename Allocator>
    array_view<T> make_array_view(std::vector<T, Allocator>& vec) noexcept
    {
        return array_view<T>(vec);
    }

    template<typename T, typename Allocator>
    array_view<T const> make_array_view(std::vector<T, Allocator> const& vec) noexcept
    {
        return array_view<T const>(vec);
    }

    /**
     * Creates an array_view into the storage of a std::array.
     */
    template<typename T, std::size_t N>
    constexpr
    array_view<T> make_array_view(std::array<T, N>& ary) noexcept
    {
        return array_view<T>(ary);
    }

    template<typename T, std::size_t N>
    constexpr
    array_view<T const> make_array_view(std::array<T, N> const& ary) noexcept
    {
        return array_view<T const>(ary);
    }

    /**
     * Creates an array_view into an array.
     */
    template<typename T, std::size_t N>
    constexpr
    array_view<T> make_array_view(T(& ary)[N]) noexcept
    {
        return array_view<T>(ary);
    }

    template<typename T, std::size_t N>
    constexpr
    array_view<T const> make_array_view(T const(& ary)[N]) noexcept
    {
        return array_view<T const>(ary);
    }

    /**
     * Creates an array_view into raw memory region. Behavior is undefined if
     * the specified memory region is invalid (hence no noexcept).
     */
    template<typename T>
    constexpr
    array_view<T> make_array_view(T* ptr, std::size_t count)
    {
        return array_view<T>(ptr, count);
    }

    template<typename T, std::size_t N>
    constexpr
    array_view<T const> make_array_view(T const* ptr, std::size_t count)
    {
        return array_view<T const>(ptr, count);
    }
}

#endif
