#ifndef EXT_SMART_POINTER_HPP
#define EXT_SMART_POINTER_HPP

#include <memory>
#include <type_traits>
#include <utility>

namespace ext
{
    /*
     * Converts a raw pointer to a non-deleting shared_ptr object pointing to
     * the same object as the raw pointer.
     *
     * Note: Using retuned shared_ptr object is no safer than using raw
     * pointer. It is programmer's responsibility to ensure that the object
     * pointed to by the raw pointer lasts until the all descendants of the
     * returned shared_ptr are destroyed.
     */
    template<typename T>
    std::shared_ptr<T> view_as_shared(T* ptr) noexcept
    {
        return std::shared_ptr<T>{std::shared_ptr<T>{}, ptr};
    }

    /*
     * Base class for polymorphically invoking copy constructor of an object
     * of a subclass of type T.
     */
    template<typename T>
    struct polymorphic_value
    {
        polymorphic_value() = default;
        polymorphic_value(polymorphic_value const&) = delete;
        polymorphic_value& operator=(polymorphic_value const&) = delete;

        virtual ~polymorphic_value() noexcept = default;
        virtual std::unique_ptr<polymorphic_value> copy() = 0;
        virtual T& value() noexcept = 0;
    };

    /*
     * Implementation of polymorphic_value<T> where the copy constructor
     * of U is properly invoked via the copy() member function.
     */
    template<typename T, typename U>
    struct concrete_value : polymorphic_value<T>
    {
        static_assert(std::is_convertible<U*, T*>::value, "U must be a subclass of T");

        explicit
        concrete_value(U const& instance)
            : value_{instance}
        {
        }

        explicit
        concrete_value(U&& instance)
            : value_{std::move(instance)}
        {
        }

        std::unique_ptr<polymorphic_value<T>> copy() override
        {
            // Get around possible non-copy constructor U(U&)
            return std::make_unique<concrete_value>(const_cast<U const&>(value_));
        }

        T& value() noexcept override
        {
            return value_;
        }

      private:
        U value_;
    };

    /*
     * Smart pointer with value semantics. Typical use would be for holding a
     * pointer to base class for pimpl idiom.
     */
    template<typename T>
    struct value_ptr
    {
        using element_type = T;
        using value_type = std::remove_const_t<T>;
        using pointer = T*;
        using reference = T&;

        /*
         * Initialises itself to the null state.
         */
        value_ptr() noexcept = default;

        /*
         * Initialises itself to the null state.
         */
        value_ptr(std::nullptr_t) noexcept
            : value_ptr{}
        {
        }

        /*
         * Constructs a value_ptr pointing to an object of given value.
         */
        template<typename U,
                 std::enable_if_t<std::is_convertible<U*, T*>::value, int> = 0>
        explicit
        value_ptr(U value)
            : store_{std::make_unique<concrete_value<T, U>>(std::move(value))}
        {
        }

        /*
         * Initialises the owned object to be a copy of the other one.
         */
        value_ptr(value_ptr const& other)
            : store_{other.store_->copy()}
        {
        }

        value_ptr& operator=(value_ptr const& other)
        {
            value_ptr{other}.swap(*this);
            return *this;
        }

        /*
         * Grabs the object owned by other value_ptr.
         */
        value_ptr(value_ptr&& other) noexcept = default;
        value_ptr& operator=(value_ptr&& other) noexcept = default;

        /*
         * Swaps the underlying pointer with that of the other object.
         */
        void swap(value_ptr& other) noexcept
        {
            store_.swap(other.store_);
        }

        /*
         * Destroys owned object, if any, and sets itself to the null state.
         */
        void reset() noexcept
        {
            store_.reset();
        }

        /*
         * value_ptr always evaluates to true (non-null) as a pointer.
         */
        explicit operator bool() const noexcept
        {
            return static_cast<bool>(store_);
        }

        /*
         * Returns a pointer to the stored object.
         */
        pointer get() const noexcept
        {
            return store_ ? &store_->value() : nullptr;
        }

        /*
         * The usual arrow operator.
         */
        pointer operator->() const noexcept
        {
            return get();
        }

        /*
         * Returns a reference to the stored object.
         */
        reference operator*() const noexcept
        {
            return *get();
        }

      private:
        std::unique_ptr<polymorphic_value<T>> store_;
    };
}

#endif
