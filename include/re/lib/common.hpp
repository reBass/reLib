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
