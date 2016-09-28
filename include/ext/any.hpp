/*
 * The any data type with narrow contract design.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_ANY_HPP
#define EXT_ANY_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include "clone_ptr.hpp"

namespace ext
{
    //--------------------------------------------------------------------------
    // Type erasure
    //--------------------------------------------------------------------------

    struct placeholder
    {
        virtual
        ~placeholder() = default;

        virtual
        std::unique_ptr<placeholder> clone() const = 0;
    };

    template<typename T>
    struct holder : placeholder
    {
        T value;

        holder(T const& init)
            : value {init}
        {
        }

        holder(T& init)
            : value {init}
        {
        }

        holder(T&& init) noexcept
            : value {std::move(init)}
        {
        }

        template<typename... Args>
        holder(Args&&... args)
            : value(std::forward<Args>(args)...)
        {
        }

        std::unique_ptr<placeholder> clone() const override
        {
            return std::make_unique<holder>(value);
        }
    };

    //--------------------------------------------------------------------------
    // Any
    //--------------------------------------------------------------------------

    /**
     * Object that holds an instance of any type.
     */
    struct any
    {
        //----------------------------------------------------------------------
        // Constructors and assignment operator
        //----------------------------------------------------------------------

        /**
         * Constructs an empty object.
         */
        any() = default;

        /**
         * Constructs an object with initial content.
         */
        template<typename T>
        any(T&& value)
            : data_ {new ext::holder<std::decay_t<T>>(std::forward<T>(value))}
        {
        }

        /**
         * Assigns specified type and value.
         */
        template<typename T>
        any& operator=(T&& value)
        {
            any(std::forward<T>(value)).swap(*this);
            return *this;
        }

        //----------------------------------------------------------------------
        // Special member functions
        //----------------------------------------------------------------------

        /**
         * Copy constructor
         */
        any(any const&) = default;
        any(any&) = default;

        /**
         * Move constructor
         */
        any(any&&) = default;

        /**
         * Copy assignment
         */
        any& operator=(any const&) = default;
        any& operator=(any&) = default;

        /**
         * Move assignment
         */
        any& operator=(any&&) = default;

        //----------------------------------------------------------------------
        // Modifiers
        //----------------------------------------------------------------------

        /**
         * Changes the contained object by constructing a new object directly.
         */
        template<typename T, typename... Args>
        void emplace(Args&&... args)
        {
            data_.reset(new ext::holder<T>(std::forward<Args>(args)...));
        }

        /**
         * Destroys contained object.
         */
        void reset() noexcept
        {
            data_.reset();
        }

        /**
         * Swaps two `any` objects.
         */
        void swap(any& other) noexcept
        {
            data_.swap(other.data_);
        }

        //----------------------------------------------------------------------
        // Observers
        //----------------------------------------------------------------------

        /**
         * Checks if object holds a value.
         */
        bool has_value() const noexcept
        {
            return data_ != nullptr;
        }

        /**
         * Checks if object holds a value of specified type.
         */
        template<typename T>
        bool has_value_of() const noexcept
        {
            return dynamic_cast<ext::holder<T>*>(data_.get()) != nullptr;
        }

        /**
         * Accesses the contained object.
         *
         * This function does not check dynamic type. Behaviour is undefined if
         * specified type does not match the dynamic type.
         */
        template<typename T>
        T& value()
        {
            return static_cast<ext::holder<T>&>(*data_).value;
        }

        template<typename T>
        T const& value() const
        {
            return static_cast<ext::holder<T>&>(*data_).value;
        }

      private:
        ext::clone_ptr<ext::placeholder> data_;
    };
}

#endif
