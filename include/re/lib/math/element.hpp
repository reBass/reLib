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

#include <cassert>

#include <algorithm>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>

namespace re {
namespace math {

template <typename T>
struct element
{
    T value;
    std::size_t index;

    element()
    {
    }

    template <typename InputIt>
    element(InputIt first, InputIt element) :
        value(*element),
        index(static_cast<std::size_t>(std::distance(first, element)))
    {
    }

    element(T& value, std::size_t index) :
        value(value),
        index(index)
    {
    }

    element(T&& value, std::size_t index) :
        value(value),
        index(index)
    {
    }

    template <typename InputIt>
    static element<T>
    max(InputIt first, InputIt last)
    {
        auto m = std::max_element(
            first,
            last
        );
        return element<T>(first, m);
    }
};

template <
    typename InputIt1,
    typename InputIt2,
    typename BinaryOp,
    typename ElementT = typename BinaryOp::result_type
>
auto max_result(
    InputIt1 first1,
    InputIt1 last1,
    InputIt2 first2,
    BinaryOp op
) noexcept
{
    if (first1 == last1) {
        return element<ElementT>(static_cast<ElementT>(0), std::size_t{0});
    }

    if (std::distance(first1, last1) == 0) {
        return element<ElementT>(op(*first1, *first2), std::size_t{0});
    }

    auto it1 = first1;
    auto it2 = first2;
    auto largest1 = first1;
    auto largest_value = op(*first1, *first2);

    while (it1 != last1) {
        auto value = op(*(++it1), *(++it2));
        if (largest_value < value) {
            largest1 = it1;
            largest_value = value;
        }
    }

    return element<ElementT>(
        largest_value,
        std::distance(first1, largest1)
    );
};

}
}
