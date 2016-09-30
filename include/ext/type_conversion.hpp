/*
 * Utility functions for simple type conversions.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_TYPE_CONVERSION_HPP
#define EXT_TYPE_CONVERSION_HPP

#include <memory>

namespace ext
{
    /**
     * View as const reference.
     */
    template<typename T>
    constexpr
    T const& as_const(T const& x) noexcept
    {
        return x;
    }

    /**
     * View raw pointer as unmanaged shared_ptr.
     *
     * Caution: The returned shared_ptr object does not own the pointer. It is
     * programmer's responsibility to correctly destroy the pointed object. Yet
     * this function may be useful to, say, passing a pointer to static object
     * to some API that expects shared_ptr as input.
     */
    template<typename T>
    std::shared_ptr<T> as_shared_ptr(T* ptr)
    {
        return std::shared_ptr<T> {std::shared_ptr<void> {}, ptr};
    }
}

#endif
