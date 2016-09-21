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

#include <tuple>
#include <type_traits>
#include <utility>

// order matters
#include <re/lib/simd/identification.hpp>
#include <re/lib/simd/intrinsics/template.hpp>

#if defined(RE_ARCH_NEON)
#include <re/lib/simd/intrinsics/neon.hpp>
#elif defined(RE_ARCH_AVX512)
#include <re/lib/simd/intrinsics/avx512.hpp>
#elif defined(RE_ARCH_AVX)
#include <re/lib/simd/intrinsics/avx.hpp>
#elif defined(RE_ARCH_SSE3)
#include <re/lib/simd/intrinsics/sse3.hpp>
#endif

//#define RE_ARCH_NEON

namespace re {
namespace simd {

template <typename T, typename BinaryOp, typename UnaryOp>
struct apply
{
    T operator()(T a, T b) {
        return BinaryOp()(a, UnaryOp()(b));
    }
    lane<T> operator()(lane<T> a, lane<T> b) {
        return BinaryOp()(a, UnaryOp()(b));
    }
    T operator<<(lane<T> a) {
        return BinaryOp() << a;
    }
};



template <
        std::size_t ...I,
        typename StepFnT,
        typename ToScalarFnT,
        typename ScalarT,
        typename...SpanTs>
inline ScalarT
reduce_impl(
        std::index_sequence<I...>,
        StepFnT step,
        ToScalarFnT to_scalar,
        ScalarT initial,
        SpanTs ...spans)
{
    auto firsts = std::make_tuple(std::begin(spans)...);
    auto const lasts  = std::make_tuple(std::end(spans)...);
    auto state = set_lane(initial);

    while (std::get<0>(firsts) != std::get<0>(lasts)) {
        state = step(state, load(&(*std::get<I>(firsts)))...);
        std::get<I...>(firsts) += width<ScalarT>;
    }

    return to_scalar(state);
}

template <typename StepFnT, typename ToScalarFnT, typename ScalarT, typename ...SpanTs>
ScalarT reduce_scalar(StepFnT step, ToScalarFnT to_scalar, ScalarT initial, SpanTs ...spans)
{
    return reduce_impl(
        std::index_sequence_for<SpanTs...>{},
        to_scalar,
        step,
        initial,
        spans...
    );
};



}
}
