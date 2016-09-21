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

#include <gsl/span>

namespace re {

template <
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType const, Count>
subspan(const gsl::span<ElementType, Extent>& s, std::ptrdiff_t offset) {
    return {
        s.data() + offset,
        Count == gsl::dynamic_extent ? s.size() - offset : Count
    };
}

template <
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<!std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType, Count>
subspan(const gsl::span<ElementType, Extent>& s, std::ptrdiff_t offset) {
    return {
        s.data() + offset,
        Count == gsl::dynamic_extent ? s.size() - offset : Count
    };
}

template <
    std::ptrdiff_t Offset,
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType const, Count>
subspan(const gsl::span<ElementType, Extent>& s) {
    return {
        s.data() + Offset,
        Count == gsl::dynamic_extent ? s.size() - Offset : Count
    };
}

template <
    std::ptrdiff_t Offset,
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<!std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType, Count>
subspan(const gsl::span<ElementType, Extent>& s) {
    return {
        s.data() + Offset,
        Count == gsl::dynamic_extent ? s.size() - Offset : Count
    };
}

}
