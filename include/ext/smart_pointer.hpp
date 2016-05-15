#ifndef EXT_SMART_POINTER_HPP
#define EXT_SMART_POINTER_HPP

#include <memory>

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
}

#endif
