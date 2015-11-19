#ifndef EXT_INTRUSIVE_VIEW
#define EXT_INTRUSIVE_VIEW

#include <iterator>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace ext
{
    template<typename Node>
    struct unrolled_list_view
    {
      private:
        using elements_type = decltype(std::declval<Node>().elements);
        using elements_index_type = decltype(std::declval<Node>().size);
        using elements_iterator = decltype(std::begin(std::declval<Node>().elements));
      public:
        using node_type = Node;
        using value_type = typename std::iterator_traits<elements_iterator>::value_type;
        using reference = typename std::iterator_traits<elements_iterator>::reference;
        using pointer = typename std::iterator_traits<elements_iterator>::pointer;

        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = unrolled_list_view::value_type;
            using reference = unrolled_list_view::reference;
            using pointer = unrolled_list_view::pointer;
            using difference_type = std::ptrdiff_t;

            iterator() = default;

          private:
            iterator(node_type* cur) noexcept
                : cur_(cur)
                , idx_(0)
            {
            }

            friend unrolled_list_view;

          public:
            friend bool operator==(iterator const& i, iterator const& j) noexcept
            {
                return i.cur_ == j.cur_ && i.idx_ == j.idx_;
            }

            friend bool operator!=(iterator const& i, iterator const& j) noexcept
            {
                return !(i == j);
            }

            reference operator*() const
            {
                return cur_->elements[idx_];
            }

            pointer operator->() const
            {
                return &**this;
            }

            iterator& operator++()
            {
                if (++idx_ == cur_->size)
                {
                    cur_ = cur_->next;
                    idx_ = 0;
                }
                return *this;
            }

            iterator operator++(int)
            {
                iterator copy(*this);
                ++*this;
                return copy;
            }

          private:
            node_type* cur_ = nullptr;
            elements_index_type idx_ = 0;
        };

        explicit
        unrolled_list_view(node_type* head) noexcept
            : head_(head)
        {
        }

        iterator begin() const noexcept
        {
            return iterator(head_);
        }

        iterator end() const noexcept
        {
            return iterator();
        }

      private:
        node_type* head_;
    };
}

#endif
