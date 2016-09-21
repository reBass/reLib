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
#include <gsl/span>
#include <numeric>

#include <re/lib/common.hpp>

namespace re {
namespace math {

template <typename T, int_t N_in, int_t N_out>
void
normalize(gsl::span<T const, N_in> in, gsl::span<T, N_out> out)
noexcept {
    assert(std::size(in) == std::size(out));
    auto sum = std::accumulate(
        std::cbegin(in),
        std::cend(in),
        static_cast<T>(0),
        [] (const auto result, const auto value) {
            return result + std::fmax(value, 0);
        }
    );

    assert(sum >= 0);
    std::transform(
        std::cbegin(in),
        std::cend(in),
        std::begin(out),
        [sum] (auto v) {
            return v / sum;
        }
    );
}

template <typename T, int_t N>
void
normalize(gsl::span<T, N> data)
noexcept {
    normalize(gsl::span<T const, N>(data), data);
}

}
}
