#pragma once

#include <arm_neon.h>
#include <re/lib/simd/intrinsics/template.hpp>

namespace re {
namespace simd
{
template <> struct vector_of<float> { using type = float32x4_t; };

namespace intrinsics {

template <>
lane<float>
zero<float>() {
    return vcombine_f32(vcreate_f32(0), vcreate_f32(0));
}

template <>
lane<float>
load<float>::operator() (float const* data) {
    return vld1q_f32(data);
}

template <>
lane<float>
add<float>::operator() (lane<float> a, lane<float> b) {
    return vaddq_f32(a, b);
}
template <>
lane<float>
add<float>::operator<< (lane<float> a) {
    auto p = vpadd_f32(vget_low_f32(a), vget_high_f32(a));
    return vget_lane_f32(p, 0) + vget_lane_f32(p, 1);
}

template <>
lane<float>
sub<float>::operator() (lane<float> a, lane<float> b) {
    return vsubq_f32(a, b);
}

template <>
lane<float>
mul<float>::operator() (lane<float> a, lane<float> b) {
    return vmulq_f32(a, b);
}

template <>
lane<float>
sqr<float>::operator() (lane<float> a) {
    return vmulq_f32(a, a);
}

template <>
lane<float>
sqrt<float>::operator() (lane<float> a) {
    return vmulq_f32(a, vrsqrteq_f32(a));
}

template <>
lane<float>
hadd<float>::operator() (lane<float> a, lane<float> b)
{
    return vcombine_f32(
        vpadd_f32(vget_low_f32(a), vget_high_f32(a)),
        vpadd_f32(vget_low_f32(b), vget_high_f32(b))
    );
}


}
}
}
