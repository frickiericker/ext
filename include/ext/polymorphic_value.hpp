#ifndef EXT_POLYMORPHIC_VALUE_HPP
#define EXT_POLYMORPHIC_VALUE_HPP

#include <memory>
#include <type_traits>
#include <utility>

namespace ext
{
    //--------------------------------------------------------------------------
    // Type erasure
    //--------------------------------------------------------------------------

    template<typename T>
    struct polymorphic_placeholder
    {
        virtual
        ~polymorphic_placeholder() = default;

        virtual
        std::unique_ptr<polymorphic_placeholder> clone() const = 0;

        virtual
        T& value() noexcept = 0;
    };

    template<typename S, typename T>
    struct polymorphic_holder : ext::polymorphic_placeholder<T>
    {
        static_assert(std::is_copy_constructible<S>::value,
                      "contained type must be copy constructible");

        polymorphic_holder(S const& value)
            : value_(value)
        {
        }

        polymorphic_holder(S&& value)
            : value_(std::move(value))
        {
        }

        template<typename... Args>
        polymorphic_holder(Args&&... args)
            : value_(std::forward<Args>(args)...)
        {
        }

        std::unique_ptr<ext::polymorphic_placeholder<T>> clone() const override
        {
            return std::make_unique<polymorphic_holder>(*this);
        }

        T& value() noexcept override
        {
            return value_;
        }

      private:
        S value_;
    };

    //--------------------------------------------------------------------------
    // polymorhic_value
    //--------------------------------------------------------------------------

    /**
     * Holds polymorphic object in copy constructible way.
     */
    template<typename T>
    struct polymorphic_value
    {
        //----------------------------------------------------------------------
        // Constructors
        //----------------------------------------------------------------------

        /**
         * Constructs an object with specified content.
         */
        template<typename S>
        polymorphic_value(S&& value)
            : holder_ {std::make_unique<
                        ext::polymorphic_holder<std::decay_t<S>, T>>(
                            std::forward<S>(value))}
        {
            ptr_ = &holder_->value();
        }

        //----------------------------------------------------------------------
        // Special member functions
        //----------------------------------------------------------------------

        /**
         * Copies content of `other` into a new instance.
         */
        polymorphic_value(polymorphic_value const& other)
            : holder_ {other.holder_->clone()}
        {
            ptr_ = &holder_->value();
        }

        polymorphic_value(polymorphic_value& other)
            : polymorphic_value {const_cast<polymorphic_value const&>(other)}
        {
        }

        /**
         * Moves content of other into a new instance.
         */
        polymorphic_value(polymorphic_value&&) = default;

        /**
         * Copies content of other into object, replacing old one.
         */
        polymorphic_value& operator=(polymorphic_value const& other)
        {
            polymorphic_value {other}.swap(*this);
            return *this;
        }

        polymorphic_value& operator=(polymorphic_value& other)
        {
            return (*this = const_cast<polymorphic_value const&>(other));
        }

        /**
         * Moves content of other into object, replacing old one.
         */
        polymorphic_value& operator=(polymorphic_value&&) = default;

        /**
         * Destroys the content, if any.
         */
        ~polymorphic_value() = default;

        //----------------------------------------------------------------------
        // Modifiers
        //----------------------------------------------------------------------

        /**
         * Replaces the contained object with the right-hand side.
         */
        template<typename S>
        polymorphic_value& operator=(S&& value)
        {
            polymorphic_value {std::forward<S>(value)}.swap(*this);
            return *this;
        }

        /**
         * Replaces the contained object with newly constructed object.
         */
        template<typename S, typename... Args>
        void emplace(Args&&... args)
        {
            holder_ = std::make_unique<ext::polymorphic_holder<S, T>>(
                    std::forward<Args>(args)...);
            ptr_ = &holder_->value();
        }

        /**
         * Swaps contained objects.
         */
        void swap(polymorphic_value& other) noexcept
        {
            std::swap(ptr_, other.ptr_);
            std::swap(holder_, other.holder_);
        }

        //----------------------------------------------------------------------
        // Observers
        //----------------------------------------------------------------------

        /**
         * Accesses contained object.
         *
         * Behaviour is undefined if this member function is called on moved-out
         * object.
         */
        T& value() noexcept
        {
            return *ptr_;
        }

        T const& value() const noexcept
        {
            return *ptr_;
        }

        /**
         * Accesses contained object as type S.
         *
         * This function does not check the dynamic type. Behaviour is undefined
         * if the dynamic type of the contained object is not `S` or a subclass
         * of `S`.
         *
         * Behaviour is undefined if this member function is called on moved-out
         * object.
         */
        template<typename S>
        S& as()
        {
            return static_cast<S&>(*ptr_);
        }

        template<typename S>
        S const& as() const
        {
            return static_cast<S&>(*ptr_);
        }

        /**
         * Accesses member of contained object.
         *
         * Behaviour is undefined if this member function is called on moved-out
         * object.
         */
        T* operator->() noexcept
        {
            return ptr_;
        }

        T const* operator->() const noexcept
        {
            return ptr_;
        }

        //----------------------------------------------------------------------
      private:
        T* ptr_;
        std::unique_ptr<ext::polymorphic_placeholder<T>> holder_;
    };

    /**
     * Compares contained values.
     *
     * Comparison is done by `operator==` overloaded for the base type `T`, not
     * the dynamic type of the contents of the operands.
     */
    template<typename T>
    bool operator==(ext::polymorphic_value<T> const& a,
                    ext::polymorphic_value<T> const& b)
    {
        return a.value() == b.value();
    }

    template<typename T>
    bool operator!=(ext::polymorphic_value<T> const& a,
                    ext::polymorphic_value<T> const& b)
    {
        return !(a == b);
    }
}

#endif
