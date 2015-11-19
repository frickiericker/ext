#ifndef EXT_MEMORY_HPP
#define EXT_MEMORY_HPP

#include <type_traits>
#include <vector>

#include <cstddef>

namespace ext
{
    template<typename T>
    struct object_pool
    {
        static_assert(std::is_default_constructible<T>::value,
                      "pooled object must be default constructible");

        explicit
        object_pool(std::size_t size = 0)
            : storage_(size)
            , used_(0)
        {
        }

        void reserve(std::size_t size)
        {
            if (size > storage_.size())
                storage_.resize(size);
        }

        T* acquire()
        {
            return &storage_[used_++];
        }

        void release()
        {
            used_ = 0;
        }

      private:

        std::vector<T> storage_;
        std::size_t used_;
    };
}

#endif
