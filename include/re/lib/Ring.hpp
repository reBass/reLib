// Copyright (c) 2016 Roman Beránek. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

namespace re
{
template <typename C>
class Ring
{
public:
    using value_type = typename C::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using const_reference = value_type const&;
    using universal_reference = value_type&&;
    using size_type = typename C::size_type;
    using difference_type = typename C::difference_type;
    using index_type = difference_type;

    using iterator = typename C::iterator;
    using const_iterator = typename C::const_iterator;
    using reverse_iterator = typename C::reverse_iterator;
    using const_reverse_iterator = typename C::const_reverse_iterator;

    Ring()
    noexcept(std::is_nothrow_default_constructible<C>::value)
    {
    }

    Ring(Ring const& ring)
    noexcept(std::is_nothrow_copy_constructible<C>::value) :
        c(ring.c)
    {
    }

    Ring(Ring&& ring)
    noexcept(std::is_nothrow_move_constructible<C>::value) :
        c(std::move(ring.c))
    {
    }

    Ring(C const& rhs)
    noexcept(std::is_nothrow_copy_constructible<C>::value) :
        c(rhs)
    {
    }

    Ring(C&& rhs)
    noexcept(std::is_nothrow_move_constructible<C>::value) :
        c(std::move(rhs))
    {
    }

    Ring& operator=(Ring const& rhs)
    noexcept(std::is_nothrow_copy_assignable<C>::value) {
        c = rhs.c;
        return *this;
    }

    Ring& operator=(Ring&& rhs)
    noexcept(std::is_nothrow_move_assignable<C>::value) {
        c = std::move(rhs.c);
        return *this;
    }

    Ring& operator=(C const& rhs)
    noexcept(std::is_nothrow_copy_assignable<C>::value) {
        c = rhs;
        return *this;
    }

    Ring& operator=(C&& rhs)
    noexcept(std::is_nothrow_move_assignable<C>::value) {
        c = std::move(rhs);
        return *this;
    }

    template <
        typename T,
        typename = std::enable_if_t<std::is_assignable<C, T>::value>
    > Ring& operator=(T&& t)
    noexcept(std::is_nothrow_assignable<C, T>::value) {
        c = std::forward<T>(t);
        return *this;
    }

    void swap(Ring& rhs)
    noexcept (c.swap(rhs.c)) {
        c.swap(rhs.c);
    }

    void swap(C& rhs)
    noexcept (c.swap(rhs)) {
        c.swap(rhs);
    }

    size_type size() const noexcept { return c.size(); }
    size_type max_size() const noexcept { return size(); }
    bool empty() const noexcept { return c.empty(); }
    pointer data() const noexcept { return c.data();  }

    void fill(const_reference value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
        std::fill(begin(), end(), value);
    }

    reference operator[](index_type i) { return c[i]; }
    const_reference operator[](index_type i) const noexcept { return c[i]; }
    reference at(index_type i) { return c.at(i); }
    const_reference at(index_type i) const { return c.at(i); }
    reference front() noexcept { return c.front(); }
    const_reference front() const noexcept { return c.front(); }
    reference back() noexcept { return c.back(); }
    const_reference back() const noexcept { return c.back(); }

    iterator begin() noexcept { return c.begin(); }
    iterator end() noexcept { return c.end(); }
    const_iterator begin() const noexcept { return c.begin(); }
    const_iterator end() const noexcept { return c.end(); }
    const_iterator cbegin() const noexcept { return c.cbegin(); }
    const_iterator cend() const noexcept { return c.cend(); }
    reverse_iterator rbegin() noexcept { return c.rbegin(); }
    reverse_iterator rend() noexcept { return c.rend(); }
    const_reverse_iterator rbegin() const noexcept { return c.rbegin(); }
    const_reverse_iterator rend() const noexcept { return c.rend(); }
    const_reverse_iterator crbegin() const noexcept { return c.crbegin(); }
    const_reverse_iterator crend() const noexcept { return c.crend(); }

    void push_back(const_reference value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
        auto value_copy{value};
        emplace_back(std::move(value_copy));
    }

    void push_back(universal_reference value)
    noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        emplace_back(std::move(value));
    }

    template <typename... From>
    reference emplace_back(From&&... from)
    noexcept(std::is_nothrow_constructible<value_type, From...>::value
             && std::is_nothrow_move_assignable<value_type>::value)
    {
        front() = value_type(std::forward<From>(from)...);
        std::rotate(begin(), begin() + 1, end());
        return back();
    }

    void push_front(const_reference value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value) {
        auto value_copy{value};
        emplace_front(std::move(value_copy));
    }

    void push_front(universal_reference value)
    noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        emplace_front(std::move(value));
    }

    template <typename... From>
    reference emplace_front(From&&... from)
    noexcept(std::is_nothrow_constructible<value_type, From...>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        back() = value_type(std::forward<From>(from)...);
        std::rotate(begin(), end() - 1, end());
        return front();
    }

    void insert(const_iterator pos, size_type n, value_type value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        difference_type d = std::distance(cbegin(), pos);
        d = std::min(d, static_cast<difference_type>(n));
        while (d --> 0) {
            insert(pos, value);
        }
    }

    template <typename InputIt>
    void insert(const_iterator pos, InputIt first, InputIt last)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        while (first != last) {
            insert(pos, *(first++));
        }
    }

    iterator insert(const_iterator pos, const_reference value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        auto value_copy{value};
        return emplace(pos, std::move(value_copy));
    }

    iterator insert(const_iterator pos, universal_reference value)
    noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        return emplace(pos, std::move(value));
    }

    void rinsert(const_iterator pos, size_type n, value_type value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        difference_type d = std::distance(pos, cend());
        d = std::min(d, static_cast<difference_type>(n));
        while (d --> 0) {
            rinsert(pos, value);
        }
    }

    template <typename InputIt>
    void rinsert(const_iterator pos, InputIt first, InputIt last)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        while (first != last) {
            rinsert(pos, *(first++));
        }
    }

    iterator rinsert(const_iterator pos, const_reference value)
    noexcept(std::is_nothrow_copy_constructible<value_type>::value
             && std::is_nothrow_move_assignable<value_type>::value) {
        auto value_copy{value};
        return remplace(pos, std::move(value_copy));
    }

    iterator rinsert(const_iterator pos, universal_reference value)
    noexcept(std::is_nothrow_move_assignable<value_type>::value) {
        return remplace(pos, std::move(value));
    }

    template <typename... From>
    iterator emplace(const_iterator pos, From&&... from)
    noexcept(std::is_nothrow_constructible<value_type, From...>::value
             && std::is_nothrow_move_assignable<value_type>::value)
    {
        auto d = std::distance(cbegin(), pos);
        if (d == 0) {
            return begin();
        }

        front() = value_type(std::forward<From>(from)...);
        if (d == 1) {
            return begin();
        }
        auto last = begin() + d;
        std::rotate(begin(), begin() + 1, last);
        return last;
    }

    template <typename... From>
    iterator remplace(const_iterator pos, From&&... from)
    noexcept(std::is_nothrow_constructible<value_type, From...>::value
             && std::is_nothrow_move_assignable<value_type>::value)
    {
        auto d = std::distance(pos, end());
        if (d == 0) {
            return end();
        }
        back() = value_type(std::forward<From>(from)...);
        if (d == 1) {
            return end() - 1;
        }
        auto first = end() - d;
        std::rotate(first, end() - 1, end()) - 1;
        return first;
    }

private:
    C c;
};
}
