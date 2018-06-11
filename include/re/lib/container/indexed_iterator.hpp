#pragma once

#include <cassert>
#include <iterator>
#include <type_traits>

namespace re {
template <typename Container, bool IsConst>
class indexed_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<
        IsConst,
        std::add_const_t<typename Container::value_type>,
        typename Container::value_type
    >;
    using difference_type = typename Container::difference_type;
    using pointer = std::add_pointer_t<value_type>;
    using reference = std::add_lvalue_reference_t<value_type>;
    using container_t = std::conditional_t<
        IsConst,
        std::add_const_t<Container>,
        Container
    >;

    constexpr indexed_iterator(
        container_t* c,
        difference_type i
    ) :
        container(c),
        index(i)
    {
        assert(i >= 0);
    }

    friend class indexed_iterator<Container, true>;
    constexpr indexed_iterator(indexed_iterator<Container, false> const& other)
    noexcept :
        indexed_iterator(other.container, other.index)
    {
    }

    constexpr reference operator*()
    const {
        return (*container)[index];
    }

    constexpr pointer operator->()
    const {
        return &((*container)[index]);
    }

    constexpr indexed_iterator& operator++()
    noexcept {
        ++index;
        return *this;
    }

    constexpr indexed_iterator operator++(int)
    noexcept {
        auto ret = *this;
        ++index;
        return ret;
    }

    constexpr indexed_iterator& operator--()
    noexcept {
        --index;
        return *this;
    }

    constexpr indexed_iterator operator--(int)
    noexcept {
        auto ret = *this;
        --index;
        return ret;
    }

    constexpr indexed_iterator operator+(difference_type n)
    const noexcept {
        auto ret = *this;
        return ret += n;
    }

    constexpr indexed_iterator& operator+=(difference_type n)
    noexcept {
        index += n;
        return *this;
    }

    constexpr indexed_iterator operator-(difference_type n)
    const noexcept {
        auto ret = *this;
        return ret -= n;
    }

    constexpr indexed_iterator& operator-=(difference_type n)
    noexcept {
        index += -n;
        return *this;
    }

    constexpr difference_type operator-(const indexed_iterator& rhs)
    const noexcept {
        assert(container == rhs.container);
        return index - rhs.index;
    }

    constexpr indexed_iterator operator[](difference_type n)
    const noexcept {
        return indexed_iterator(container, index + n);
    }

    constexpr friend bool operator==(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        return lhs.container == rhs.container && lhs.index == rhs.index;
    }

    constexpr friend bool operator!=(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        return !(lhs == rhs);
    }

    constexpr friend bool operator<(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        assert(lhs.container == rhs.container);
        return lhs.index < rhs.index;
    }

    constexpr friend bool operator<=(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        return !(rhs < lhs);
    }

    constexpr friend bool operator>(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        return rhs < lhs;
    }

    constexpr friend bool operator>=(
        indexed_iterator const& lhs,
        indexed_iterator const& rhs
    ) noexcept {
        return !(rhs > lhs);
    }

protected:
    container_t* container;
    difference_type index;
};

template <typename Container, bool IsConst>
constexpr indexed_iterator<Container, IsConst> operator+(
    typename indexed_iterator<Container, IsConst>::difference_type n,
    indexed_iterator<Container, IsConst> const& rhs
) noexcept {
    return rhs + n;
}

template <typename Container, bool IsConst>
constexpr indexed_iterator<Container, IsConst> operator-(
    typename indexed_iterator<Container, IsConst>::difference_type n,
    const indexed_iterator<Container, IsConst>& rhs
) noexcept {
    return rhs - n;
}
}

/*
template <bool IsConst = false>
class iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<
        IsConst,
        std::add_const_t<value_type>,
        value_type
    >;
    using pointer = std::add_pointer_t<value_type>;
    using reference = std::add_lvalue_reference_t<value_type>;

    constexpr iterator(
        std::conditional_t<
            IsConst,
            ring_array const&,
            ring_array&
        > ring,
        size_type position
    ) noexcept :
        ring(ring),
        position(position) {
    }

    constexpr iterator(
        iterator const& it,
        difference_type diff
    ) noexcept :
        ring(it.ring),
        position(it.position + diff) {
    }

    TODO
    iterator<true>(iterator<false> const& non_const_iterator) {
        return iterator<true>(
            non_const_iterator.ring,
            non_const_iterator.position
        );
    }


    constexpr iterator& operator++() noexcept {
        ++position;
        return *this;
    }
    constexpr iterator& operator++(int) noexcept {
        ++position;
        return *this;
    }
    constexpr iterator& operator--() noexcept {
        --position;
        return *this;
    }
    constexpr iterator& operator--(int) noexcept {
        --position;
        return *this;
    }
    constexpr iterator operator+(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return iterator(ring, position + rhs.position);
    }
    constexpr iterator operator-(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return iterator(ring, position - rhs.position);
    }
    constexpr iterator operator+(difference_type rhs) const noexcept {
        return iterator(ring, position + rhs);
    }
    constexpr iterator& operator+=(difference_type rhs) noexcept {
        position += rhs;
        return *this;
    }
    constexpr iterator operator-(difference_type rhs) const noexcept {
        return iterator(ring, position - rhs);
    }
    constexpr iterator& operator-=(difference_type rhs) noexcept {
        position -= rhs;
        return *this;
    }
    constexpr bool operator==(iterator const& rhs) const noexcept {
        return (&ring == &rhs.ring) && (position == rhs.position);
    }
    constexpr bool operator!=(iterator const& rhs) const noexcept {
        return (&ring != &rhs.ring) || (position != rhs.position);
    }
    constexpr bool operator<(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return position < rhs.position;
    }
    constexpr bool operator<=(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return position <= rhs.position;
    }
    constexpr bool operator>(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return position > rhs.position;
    }
    constexpr bool operator>=(iterator const& rhs) const noexcept {
        assert(&ring == &rhs.ring);
        return position >= rhs.position;
    }
    constexpr reference
    operator[](difference_type d) const noexcept {
        return ring[static_cast<size_type>(position + d)];
    }
    constexpr reference
    operator*() const noexcept {
        return ring[position];
    }
    constexpr pointer
    operator->() const noexcept {
        return &ring[position];
    }
private:
    std::conditional_t<IsConst, ring_array const&, ring_array&> ring;
    size_type position;
};
*/
