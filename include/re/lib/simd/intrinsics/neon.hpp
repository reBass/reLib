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

#include <arm_neon.h>
#include <re/lib/simd/intrinsics/template.hpp>

namespace re {
namespace simd
{
template <> struct vector_of<float> { using type = float32x4_t; };

namespace intrinsics {

template <>
lane<float>
load<float>::operator() (float const* data) {
    return vld1q_f32(data);
}

template <>
lane<float>
zero<float>::operator() () {
    return vcombine_f32(vcreate_f32(0), vcreate_f32(0));
}

template <>
lane<float>
add<float>::operator() (lane<float> a, lane<float> b) {
    return vaddq_f32(a, b);
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
