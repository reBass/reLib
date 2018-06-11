#pragma once

#include <cassert>
#include <gsl/span>

#include <re/lib/common.hpp>
#include <re/lib/container/ring_array.hpp>
#include <re/lib/math/mean.hpp>

namespace re {
namespace math {

template <typename T, int_t N_in, int_t N_out, int_t N_thresh>
void threshold_reference(
    gsl::span<T const, N_in> in,
    gsl::span<T, N_out> out,
    gsl::span<T, N_thresh> thresh,
    int_t radius
) noexcept {
    assert(
        std::size(in) == std::size(out)
        && std::size(out) == std::size(thresh)
    );
    auto const n = std::size(in);

    for (int_t i = 0; i < n; ++i) {
        auto offset = std::max(i - radius, int_t{0});
        auto count = std::min(n - offset, 2*radius);
        count = std::min(count, i + radius);
        thresh[i] = mean(in.subspan(offset, count));
    }

    std::transform(
        std::cbegin(in),
        std::cend(in),
        std::cbegin(thresh),
        std::begin(out),
        [] (auto value, auto threshold) {
            return std::fdim(value, threshold);
        }
    );
}

template <int_t Radius, typename T, int_t N_in, int_t N_out>
void
adaptive_threshold(
    gsl::span<T const, N_in> in,
    gsl::span<T, N_out> out
) noexcept {
    assert(std::size(in) == std::size(out));
    auto const n = std::size(in);

    assert(n >= 2*Radius);
    T sum = 0;
    int_t count = 0;
    ring_array<T, Radius> temp;

    for (auto i = 0; i < n + Radius; ++i) {
        if (i < n) {
            sum += in[i];
            ++count;
        }
        if (i >= Radius) {
            auto const in_value = in[i - Radius];
            out[i - Radius] = std::fdim(in_value, sum / count);
            if (i > 2*Radius) {
                sum -= temp.front();
                --count;
            }
            temp.push_back(in_value);
        }
    }
}

template <int_t Radius, typename T, int_t N>
void
adaptive_threshold(gsl::span<T, N> data)
noexcept {
    adaptive_threshold<Radius, T, N>(data, data);
}


}
}
