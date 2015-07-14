#ifndef EXT_META_HPP
#define EXT_META_HPP

namespace ext
{
    template<typename T>
    struct make_non_deduced
    {
        using type = T;
    };

    template<typename T>
    using make_non_deduced_t = typename make_non_deduced<T>::type;
}

#endif
