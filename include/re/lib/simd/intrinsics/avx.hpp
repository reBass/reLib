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
#include <complex>

namespace re {
namespace simd
{
template <> struct vector_of<float> { using type = __m256; };
template <> struct vector_of<double> { using type = __m256d; };
template <> struct vector_of<std::complex<float>> { using type = __m256; };

namespace intrinsics {

template <>
lane<float>
zero<float>() {
    return _mm256_setzero_ps();
}

template <>
lane<double>
zero<double>() {
    return _mm256_setzero_pd();
}

template <>
lane<float>
set<float>::operator() (float value)
{
    return _mm256_set1_ps(value);
}

template <>
lane<double>
set<double>::operator() (double value)
{
    return _mm256_set1_pd(value);
}


template <>
lane<float>
load<float>::operator() (lane_ptr<float const> p)
{
    return _mm256_loadu_ps(p);
}

template <>
lane<double>
load<double>::operator() (lane_ptr<double const> p)
{
    return _mm256_loadu_pd(p);
}

template <>
lane<float>
add<float>::operator() (lane<float> a, lane<float> b)
{
    return _mm256_add_ps(a, b);
}

template <>
lane<double>
add<double>::operator() (lane<double> a, lane<double> b)
{
    return _mm256_add_pd(a, b);
}

template <>
float add<float>::operator() (lane<float> a)
{
    a = _mm256_hadd_ps(a, a);
    a = _mm256_hadd_ps(a, a);
    a = _mm256_hadd_ps(a, a);
    return a.a[0];
}

template <>
double add<double>::operator() (lane<double> a)
{
    a = _mm256_hadd_pd(a, a);
    a = _mm256_hadd_pd(a, a);
    return a.a[0];
}

template <>
lane<float>
sub<float>::operator() (lane<float> a, lane<float> b)
{
    return _mm256_sub_ps(a, b);
}

template <>
lane<double>
sub<double>::operator() (lane<double> a, lane<double> b)
{
    return _mm256_sub_pd(a, b);
}

template <>
lane<float>
mul<float>::operator() (lane<float> a, lane<float> b)
{
    return _mm256_mul_ps(a, b);
}

template <>
lane<double>
mul<double>::operator() (lane<double> a, lane<double> b)
{
    return _mm256_mul_pd(a, b);
}

template <>
lane<float>
sqr<float>::operator() (lane<float> a)
{
    return _mm256_mul_ps(a, a);
}

template <>
lane<double>
sqr<double>::operator() (lane<double> a)
{
    return _mm256_mul_pd(a, a);
}

template <>
lane<float>
sqrt<float>::operator() (lane<float> a)
{
    return _mm256_sqrt_ps(a);
}

template <>
lane<double>
sqrt<double>::operator() (lane<double> a)
{
    return _mm256_sqrt_pd(a);
}

template <>
lane<float>
hadd<float>::operator() (lane<float> a, lane<float> b)
{
    return _mm256_hadd_ps(a, b);
}

template <>
lane<double>
hadd<double>::operator() (lane<double> a, lane<double> b)
{
    return _mm256_hadd_pd(a, b);
}

template <>
lane<float>
max<float>::operator() (lane<float> a, lane<float> b)
{
    return _mm256_max_ps(a, b);
}

template <>
lane<double>
max<double>::operator() (lane<double> a, lane<double> b)
{
    return _mm256_max_pd(a, b);
}


template <>
lane <std::complex<float>>
add<std::complex<float>>::operator() (
    lane <std::complex<float>> a,
    lane <std::complex<float>> b
)
{
    return _mm256_add_ps(a, b);
}

template <>
lane <std::complex<float>>
sub<std::complex<float>>::operator() (
    lane <std::complex<float>> a,
    lane <std::complex<float>> b
)
{
    return _mm256_sub_ps(a, b);
}

template <>
lane <std::complex<float>>
mul<std::complex<float>>::operator()(
    lane <std::complex<float>> a,
    lane <std::complex<float>> b
)
{
    auto ac_bd = _mm256_mul_ps(a, b);
    b = __builtin_shufflevector(b.v, b.v, 1, 0, 3, 2, 5, 4, 7, 6);
    auto ad_bc = _mm256_mul_ps(a, b);
    a.v = __builtin_shufflevector(ac_bd, ad_bc, 0, 8, 2, 10, 4, 12, 6, 14);
    b.v = __builtin_shufflevector(ac_bd, ad_bc, 1, 9, 3, 11, 5, 13, 7, 15);

    return _mm256_addsub_ps(a, b);
}

template <>
inline lane<std::complex<float>>
load<std::complex<float>>::operator()(lane_ptr<std::complex<float> const> p) {
    return _mm256_load_ps(reinterpret_cast<float const*>(p.ptr));
}

template <>
inline void
store<std::complex<float>>::operator()(
    lane_ptr<std::complex<float>> p,
    lane<std::complex<float>> value
) {
    _mm256_store_ps(reinterpret_cast<float*>(p.ptr), value);
}


}

}
}
