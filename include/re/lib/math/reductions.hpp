#pragma once

#include <tuple>
#include <utility>
#include <limits>
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
    for (auto i = 0u; i < (in.size() % width<T>); ++i) {
        result.a[i] = op(result.a[i], *it);
    }
    for (; it < std::cend(in); it += width<T>) {
        result = op(result, load(&(*it)));
    }
    return op << result;
};

template <typename T, int_t N>
std::pair<T, int_t> max(gsl::span<T const, N> in) {
    auto it = std::cbegin(in);
    auto max = intrinsics::fill<T>()(std::numeric_limits<T>::lowest());
    for (auto i = 0u; i < (in.size() % width<T>); ++i) {
        max.a[i] = intrinsics::max<T>()(max.a[i], *(it++));
    }
    for (auto const end = std::cend(in); it < end; it += width<T>) {
        auto temp = intrinsics::max<T>()(max, intrinsics::load<T>()(&(*it)));
        if (intrinsics::any_greater_than<T>()(temp, max)) {

        }
    }
};

template <typename T, int_t N>
T sum(gsl::span<T const, N> in) {
    return reduce(in, intrinsics::zero<T>(), intrinsics::add<T>());
}

template <typename T, int_t N>
T mean(gsl::span<T const, N> in) {
    return sum(in) / in.size();
}

template <typename T, int_t N>
T sum_of_squares(gsl::span<T const, N> in) {
    return reduce(
        in,
        intrinsics::zero<T>(),
        apply<T, intrinsics::add<T>, intrinsics::sqr<T>>()
    );
};

}
}
