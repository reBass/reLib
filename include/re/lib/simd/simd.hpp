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
