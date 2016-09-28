/*
 * Smart pointer that duplicates pointed object on copy.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_CLONE_PTR_HPP
#define EXT_CLONE_PTR_HPP

#include <memory>
#include <utility>
#include <cassert>

namespace ext
{
    /**
     * Customizable traits class for cloning polymorphic object whose dynamic
     * type is derived from type `T`.
     */
    template<typename T>
    struct clone_traits
    {
        /**
         * Creates a copy of the given object using its dynamic type.
         *
         * This default implementation assumes the existence of `T::clone()`
         * member function and uses it to create a copy, or compile fails.
         * Assertion fails if that function returns a null pointer.
         *
         * @param object
         *      A reference to the object to make a copy of.
         *
         * @returns
         *      A non-null owning pointer to the created copy.
         */
        static
        std::unique_ptr<T> clone(T const& object)
        {
            std::unique_ptr<T> copy = object.clone();
            assert(static_cast<bool>(copy));
            return copy;
        }
    };

    /**
     * Smart pointer with value semantics using polymorphic clone function.
     */
    template<typename T>
    struct clone_ptr
    {
        //----------------------------------------------------------------------
        // Constructors
        //----------------------------------------------------------------------

        /**
         * Initializes to null by default.
         */
        clone_ptr() = default;

        /**
         * Initializes to null.
         */
        clone_ptr(std::nullptr_t)
        {
        }

        /**
         * Initializes to the given pointer.
         */
        explicit
        clone_ptr(T* ptr)
            : ptr_ {ptr}
        {
        }

        //----------------------------------------------------------------------
        // Special member functions
        //----------------------------------------------------------------------

        /**
         * Transfers ownership from another `clone_ptr` object.
         */
        clone_ptr(clone_ptr&& other) = default;

        /**
         * Initializes as a pointer to a copy of anohter object.
         *
         * If the passed in pointer is non-null, this constructor creates a copy
         * of the pointed object is created using `ext::clone_traits<T>::clone()`
         * function and owns the returned pointer.
         */
        clone_ptr(clone_ptr const& other)
            : ptr_ {other ? ext::clone_traits<T>::clone(*other.ptr_) : nullptr}
        {
        }

        /**
         * Assigns the `clone_ptr`.
         */
        clone_ptr& operator=(clone_ptr&& other) = default;

        /**
         * Assigns the `clone_ptr`.
         */
        clone_ptr& operator=(clone_ptr const& other)
        {
            clone_ptr(other).swap(*this);
            return *this;
        }

        /**
         * Destructs the managed object if such is present.
         */
        ~clone_ptr() = default;

        //----------------------------------------------------------------------
        // Observers
        //----------------------------------------------------------------------

        /**
         * Returns a pointer to the managed object.
         */
        T* get() const noexcept
        {
            return ptr_.get();
        }

        /**
         * Checks if object has associated managed object.
         */
        explicit
        operator bool() const noexcept
        {
            return static_cast<bool>(ptr_);
        }

        /**
         * Dereferences pointer to the managed object.
         */
        T& operator*() const
        {
            return *ptr_;
        }

        T* operator->() const
        {
            return ptr_.get();
        }

        //----------------------------------------------------------------------
        // Modifiers
        //----------------------------------------------------------------------

        /**
         * Returns a pointer to the managed object and releases the ownership.
         */
        T* release() noexcept
        {
            return ptr_.release();
        }

        /**
         * Replaces the managed object.
         */
        void reset() noexcept
        {
            ptr_.reset();
        }

        void reset(std::nullptr_t) noexcept
        {
            ptr_.reset(nullptr);
        }

        void reset(T* ptr) noexcept
        {
            ptr_.reset(ptr);
        }

        /**
         * Swaps the managed objects.
         */
        void swap(clone_ptr& other) noexcept
        {
            ptr_.swap(other.ptr_);
        }

      private:
        std::unique_ptr<T> ptr_;
    };

    template<typename T>
    bool operator==(ext::clone_ptr<T> p, ext::clone_ptr<T> q) noexcept
    {
        return p.get() == q.get();
    }

    template<typename T>
    bool operator!=(ext::clone_ptr<T> p, ext::clone_ptr<T> q) noexcept
    {
        return !(p == q);
    }

    template<typename T>
    bool operator==(ext::clone_ptr<T> p, std::nullptr_t) noexcept
    {
        return !p;
    }

    template<typename T>
    bool operator!=(ext::clone_ptr<T> p, std::nullptr_t) noexcept
    {
        return !(p == nullptr);
    }

    template<typename T>
    bool operator==(std::nullptr_t, ext::clone_ptr<T> p) noexcept
    {
        return p == nullptr;
    }

    template<typename T>
    bool operator!=(std::nullptr_t, ext::clone_ptr<T> p) noexcept
    {
        return !(p == nullptr);
    }
}

#endif
