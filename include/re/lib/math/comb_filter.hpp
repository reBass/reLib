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
#include <re/lib/math/mean.hpp>

namespace re {
namespace math {

template <typename T, int_t N_in, int_t N_out>
void
comb_filter(gsl::span<T const, N_in> in, gsl::span<T, N_out> out)
noexcept {
    static_assert(N_in >= 0 && N_out >= 0);
    static_assert(N_in % N_out == 0);

    constexpr auto n_new =  (N_in > N_out) ? N_in - N_out : 0;
    constexpr auto stage = N_in / N_out;

    if (stage > 1) {
        comb_filter(gsl::span<T const, n_new>(in.data(), n_new), out);
    } else {
        std::fill(std::begin(out), std::end(out), 0);
    }

    for (auto i = 1; i < N_out; ++i) {
        out[i] += mean(in.subspan(stage * (i - 1) + 1, 2*stage - 1));
    }
}

}
}
