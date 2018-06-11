#pragma once

#include <complex>
#include <tuple>
#include <utility>

namespace re {
namespace fft {

enum class direction: bool {
    forward = false,
    inverse = true
};

template <typename T>
constexpr void
scissors(std::complex<T>& a, std::complex<T>& b) {
    std::tie(a, b) = std::make_pair(a + b, a - b);
}

constexpr bool
is_forward(direction d) {
    return d == direction::forward;
}
constexpr bool
is_inverse(direction d) {
    return d == direction::inverse;
}

template <typename T>
std::complex<T>
inline multiply_fast(std::complex<T> a, std::complex<T> b) noexcept
{
    return {
        a.real() * b.real() - a.imag() * b.imag(),
        a.real() * b.imag() + a.imag() * b.real()
    };
}

template <bool IsInverse, typename T>
constexpr std::complex<T> flip(std::complex<T> const value)
noexcept {
    return IsInverse
           ? std::complex<T>{ -value.imag(), value.real() }
           : std::complex<T>{ value.imag(), -value.real() };
}

template <direction Direction, typename T>
constexpr std::complex<T> flip(std::complex<T> value)
noexcept {
    return Direction == direction::forward
           ? std::complex<T>{  value.imag(), -value.real() }
           : std::complex<T>{ -value.imag(),  value.real() };
}

}
}
