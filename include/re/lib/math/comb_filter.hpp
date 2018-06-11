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
