#pragma once

#include <cassert>
#include <numeric>
#include <gsl/span>

#include <re/lib/common.hpp>

namespace re {
namespace math {


template <typename T, int_t N>
T
mean(gsl::span<T const, N> input)
noexcept
{
    assert(std::size(input) > 0);
    return std::accumulate(
        std::cbegin(input),
        std::cend(input),
        static_cast<T>(0)
    ) / std::size(input);
}

}
}
