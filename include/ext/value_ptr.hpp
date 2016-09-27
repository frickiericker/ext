/*
 * Smart pointer that duplicates pointed object on copy.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_VALUE_PTR_HPP
#define EXT_VALUE_PTR_HPP

#include "clone_ptr.hpp"

namespace ext
{
    template<typename T>
    struct value_ptr
    {
      private:
        ext::clone_ptr<T> ptr_;
    };
}

#endif
