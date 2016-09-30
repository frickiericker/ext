#ifndef EXT_LIFETIME_UTILITY_HPP
#define EXT_LIFETIME_UTILITY_HPP

#include <memory>

namespace ext
{
    /**
     * Counts number of live copies.
     *
     * Count starts with one at default construction. The number is increased
     * by one on copy construction or assignemnt and decreased by one on
     * destruction. It is invariant on move operation.
     *
     * The variable is shared among all copies of the original object.
     */
    struct copy_counter
    {
        /**
         * Returns the number of live copies of the original instance.
         */
        long count() const noexcept
        {
            return counter_.use_count();
        }

      private:
        std::shared_ptr<int> counter_ = std::make_shared<int>();
    };
}

#endif
