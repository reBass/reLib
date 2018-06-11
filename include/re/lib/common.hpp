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

#include <cstddef>
#include <gsl/gsl_assert>
#include <gsl/span>

template <std::size_t N>
using ivec_t __attribute__((vector_size(4*N))) = int;

#ifdef __clang__
#define RE_SHUFFLE(size, x, y, mask...) __builtin_shufflevector(x, y, mask)
#else
#define RE_SHUFFLE(size, x, y, mask...) __builtin_shuffle(x, y, ivec_t<size> { mask })
#endif

namespace re {

using int_t = std::ptrdiff_t;
using uint_t = std::size_t;

template <typename T> constexpr T pi = T(3.141592653589793238462643L);

template <typename T>
T abs_difference (const T a, const T b)
noexcept {
    return (a > b) ? (a - b) : (b - a);
}

}
