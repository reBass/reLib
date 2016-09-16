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

#include <pmmintrin.h>
#include <re/lib/simd/intrinsics/template.hpp>

namespace re {
namespace simd
{
template <> struct vector_of<float> { using type = __m128; };
template <> struct vector_of<double> { using type = __m128d; };

namespace intrinsics {

template <> lane<float>
zero<float>::operator()() {
    return _mm_setzero_ps();
}

template <> lane<double>
zero<double>::operator()() {
    return _mm_setzero_pd();
}

template <> lane<float>
add<float>::operator()(lane<float> a, lane<float> b) {
    return _mm_add_ps(a, b);
}
template <> lane<double>
add<double>::operator()(lane<double> a, lane<double> b) {
    return _mm_add_pd(a, b);
}
template <> float
add<float>::operator<<(lane<float> a) {
    a = _mm_hadd_ps(a, a);
    return _mm_hadd_ps(a, a);
}
template <> double
add<double>::operator<<(lane<double> a) {
    return _mm_hadd_pd(a, a);
}

template <> lane<float>
sub<float>::operator()(lane<float> a, lane<float> b) {
    return _mm_sub_ps(a, b);
}
template <>
lane<double>
sub<double>::operator()(lane<double> a, lane<double> b) {
    return _mm_sub_pd(a, b);
}

template <>
lane<float>
mul<float>::operator()(lane<float> a, lane<float> b) {
    return _mm_mul_ps(a, b);
}
template <>
lane<double>
mul<double>::operator()(lane<double> a, lane<double> b) {
    return _mm_mul_pd(a, b);
}

template <>
lane<float>
sqr<float>::operator()(lane<float> a) {
    return _mm_mul_ps(a, a);
}
template <>
lane<double>
sqr<double>::operator()(lane<double> a) {
    return _mm_mul_pd(a, a);
}

template <>
lane<float>
sqrt<float>::operator()(lane<float> a) {
    return _mm_sqrt_ps(a);
}
template <>
lane<double>
sqrt<double>::operator()(lane<double> a) {
    return _mm_sqrt_pd(a);
}

template <>
lane<float>
hadd<float>::operator()(lane<float> a, lane<float> b) {
    return _mm_hadd_ps(a, b);
}
template <>
lane<double>
hadd<double>::operator()(lane<double> a, lane<double> b) {
    return _mm_hadd_pd(a, b);
}

}

}
}
