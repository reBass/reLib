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

template <>
bool
any_greater_than<float>::operator()(lane<float> a, lane<float> b) {
    return _mm_movemask_ps(_mm_cmpgt_ps(a, b)) > 0;
}

}

}
}
