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

#include <immintrin.h>
#include <re/lib/simd/intrinsics/template.hpp>

namespace re {
namespace simd
{
template <> struct vector_of<float> { using type = __m512; };
template <> struct vector_of<double> { using type = __m512d; };

namespace intrinsics {

template <>
lane<float>
zero<float>() {
    return _mm512_setzero_ps();
}

template <>
lane<double>
zero<double>() {
    return _mm512_setzero_pd();
}

template <>
lane<float>
add<float>::operator()(lane<float> a, lane<float> b) {
    return _mm512_add_ps(a, b);
}
template <>
lane<double>
add<double>::operator()(lane<double> a, lane<double> b) {
    return _mm512_add_pd(a, b);
}

template <>
lane<float>
sub<float>::operator()(lane<float> a, lane<float> b) {
    return _mm512_sub_ps(a, b);
}
template <>
lane<double>
sub<double>::operator()(lane<double> a, lane<double> b) {
    return _mm512_sub_pd(a, b);
}

template <>
lane<float>
mul<float>::operator()(lane<float> a, lane<float> b) {
    return _mm512_mul_ps(a, b);
}
template <>
lane<double>
mul<double>::operator()(lane<double> a, lane<double> b) {
    return _mm512_mul_pd(a, b);
}

template <>
lane<float>
sqr<float>::operator()(lane<float> a) {
    return _mm512_mul_ps(a, a);
}
template <>
lane<double>
sqr<double>::operator()(lane<double> a) {
    return _mm512_mul_pd(a, a);
}

template <>
lane<float>
sqrt<float>::operator()(lane<float> a) {
    return _mm512_sqrt_ps(a);
}
template <>
lane<double>
sqrt<double>::operator()(lane<double> a) {
    return _mm512_sqrt_pd(a);
}

template <>
lane<float>
hadd<float>::operator()(lane<float> a, lane<float> b) {
    auto even = __builtin_shufflevector(
        a.v, b.v, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30
    );
    auto odd = __builtin_shufflevector(
        a.v, b.v, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31
    );
    return _mm512_add_ps(even, odd);
}
template <>
lane<double>
hadd<double>::operator()(lane<double> a, lane<double> b) {
    auto even = __builtin_shufflevector(a.v, b.v, 0, 2, 4, 6, 8, 10, 12, 14);
    auto odd = __builtin_shufflevector(a.v, b.v, 1, 3, 5, 7, 9, 11, 13, 15);
    return _mm512_add_pd(even, odd);
}

}
}
}
