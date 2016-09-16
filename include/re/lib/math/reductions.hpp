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

#include <re/lib/common.hpp>
#include <re/lib/simd/simd.hpp>

namespace re {
namespace math
{
using namespace simd;

template <typename T, int_t N, typename BinaryOp>
T reduce(gsl::span<T const, N> in, lane<T> result, BinaryOp op) {
    auto it = std::cbegin(in);
    for (auto i = 0u; i < (std::size(in) % width<T>); ++i) {
        result.a[i] = op(result.a[i], *(it++));
    }
    for (; it < std::cend(in); it += width<T>) {
        result = op(result, intrinsics::load<T>()(&(*it)));
    }
    return op << result;
};

template <typename T, int_t N>
T sum(gsl::span<T const, N> in) {
    return reduce(in, intrinsics::zero<T>()(), intrinsics::add<T>());
}

template <typename T, int_t N>
T mean(gsl::span<T const, N> in) {
    return sum(in) / std::size(in);
}

template <typename T, int_t N>
T sum_of_squares(gsl::span<T const, N> in) {
    return reduce(
        in,
        intrinsics::zero<T>()(),
        apply<T, intrinsics::add<T>, intrinsics::sqr<T>>()
    );
};

}
}
