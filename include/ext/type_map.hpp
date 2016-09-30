/*
 * Associates type and value.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_TYPE_MAP_HPP
#define EXT_TYPE_MAP_HPP

#include <utility>
#include <vector>
#include <cstddef>


namespace ext
{
    /**
     * Minimal perfect hash for static types.
     */
    struct type_directory
    {
        /**
         * The hash code for the static type `T`.
         *
         * Note: this cannot be variable template due to bugs in compilers.
         */
        template<typename T>
        struct index
        {
            static std::size_t const value;
        };

        /**
         * The number of registered types.
         *
         * This function may not give the correct number until all namespace-
         * scope static variables are initialized. After that, however, the
         * number gives the exclusive upper bound for all the possible hash
         * codes.
         */
        static
        std::size_t size() noexcept
        {
            return counter();
        }

      private:
        static
        std::size_t& counter() noexcept
        {
            static std::size_t counter_;
            return counter_;
        }
    };

    template<typename T>
    std::size_t const type_directory::index<T>::value
        = type_directory::counter()++;

    /**
     * Collection of `T` values associated to static types.
     *
     * === Example ===
     *
     * ```
     * ext::type_map<std::string> map;
     *
     * map.associate<char>("char");
     * map.associate<short>("short");
     * map.associate<int>("int");
     *
     * assert(map.value<char>() == "char");
     * assert(map.value<short>() == "short");
     * assert(map.value<int>() == "int");
     * ```
     *
     * === Implementation note ===
     *
     * As for the current implementation, the value type `T` needs to be default
     * constructible and move assignable.
     */
    template<typename T>
    struct type_map
    {
        using value_type = T;

        type_map()
            : entries_(ext::type_directory::size())
        {
        }

        /**
         * Associates a new object to the `Key` type.
         *
         * This function may throw an exception due to memory exhaustion or
         * failed construction of the associated object.
         *
         * @param args
         *      Arguments forwarded to the constructor of the associated object.
         *
         * @returns
         *      A reference to the associated object. The returned reference may
         *      be invalidated on subsequent invocations to `make_slot()`.
         */
        template<typename Key, typename... Args>
        value_type& associate(Args&&... args)
        {
            std::size_t const index = ext::type_directory::index<Key>::value;
            return entries_[index] = value_type(std::forward<Args>(args)...);
        }

        //@{
        /**
         * Accesses the value associated to `Key` type.
         *
         * @param Key
         *      The key must have an associated object. Otherwise the behaviour
         *      is undefined.
         *
         * @returns
         *      A reference to the associated object. The returned reference may
         *      be invalidated on subsequent invocations to `make_slot()`.
         */
        template<typename Key>
        value_type& value() noexcept
        {
            return entries_[type_directory::index<Key>::value];
        }

        template<typename Key>
        value_type const& value() const noexcept
        {
            return entries_[type_directory::index<Key>::value];
        }
        //@}

      private:
        std::vector<T> entries_;
    };
}

#endif
