#pragma once

#include <complex>
#include <re/lib/common.hpp>
#include <re/lib/container/subspan.hpp>

#include <re/lib/simd/simd.hpp>
#include "common.hpp"

namespace re {
namespace fft {

template <typename T, int_t N>
class simd_fft {
    using real_t = T;
    using cpx_t = std::complex<T>;

    static_assert(std::is_floating_point<T>::value);
    static_assert((N & (N - 1)) == 0, "N must be a power of 2.");
    static_assert(N >= 2*simd::width<cpx_t>);

    static constexpr int_t stride(int_t n_out) {
        return (n_out > 0) ? N / n_out : 0;
    }
    static constexpr int_t radix(int_t n_out) {
        return /*(n_out % 8 == 0) ? 8 : ((n_out % 4 == 0) ? 4 : 2)*/ 2;
    }
    static constexpr int_t remainder(int_t n_out) {
        return n_out / radix(n_out);
    }
    static constexpr int_t remainder() {
        return remainder(N);
    }

public:
    simd_fft()
    noexcept {
        auto const step = (false ? 2 : -2) * pi<real_t> / N;
        for (auto i = 0u; i < twiddles.size(); ++i) {
            twiddles[i] = std::polar(real_t{1}, i * step);
        }
    }

    void
    operator()(gsl::span<cpx_t const, N> in, gsl::span<cpx_t, N> out)
    noexcept {
        copy_input(in, out);
        loop(out);
    }

private:


#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"
    template <int_t N_in, int_t N_out>
    inline typename std::enable_if_t<(N_out > 2)>
    copy_input(
        gsl::span<cpx_t const, N_in> input,
        gsl::span<cpx_t, N_out> output
    ) const noexcept {
        copy_input(
            subspan<0, N_in - stride(N_out)>(input),
            subspan<0, N_out/2>(output)
        );
        copy_input(
            subspan<stride(N_out), N_in - stride(N_out)>(input),
            subspan<N_out/2, N_out/2>(output)
        );
    }
#pragma clang diagnostic pop

    template <int_t N_in, int_t N_out>
    inline typename std::enable_if_t<(N_out == 2)>
    copy_input(
        gsl::span<cpx_t const, N_in> input,
        gsl::span<cpx_t, N_out> output
    ) const noexcept {
        output[0] = input[0];
        output[1] = input[stride(N_out)];
    }

    void loop(gsl::span<cpx_t, N> output) noexcept {
        for (auto i = 0; (1 << i) < N; ++i) {
            set_twiddles(i);
            switch (i) {
                case 0:
                    stage0(output);
                    break;
                case 1:
                    stage1(output);
                    break;
                case 2:
                    stage2(output);
                    break;
                default:
                    stage_2n(output, i++);
                    break;
            }
        }
    }

    void set_twiddles(int_t stage) noexcept {
        auto const n = 1 << stage;
        auto const stride = remainder() >> stage;

        for (auto i = 0; i < n; ++i) {
            w_twiddles[i] = twiddles[i * stride];
        }
        /*
        for (auto i = 1; i * n < simd::width<cpx_t>; ++i) {
            for (auto j = 0; j < n; ++j) {
                w_twiddles[i * n + j] = w_twiddles[j];
            }
        }
         */
    }

    void stage_n(gsl::span<cpx_t, N> out, int_t stage) const noexcept {
        auto const stride = 1 << stage;

        for (auto j = 0; j < stride; j += simd::width<cpx_t>) {
            auto w = simd::load(&w_twiddles[j]);
            for (auto i = 0; i < N; i += 2*stride) {
                auto x_a = simd::load(&out[j + i]);
                auto x_b = simd::load(&out[j + i + stride]);

                auto w_x_b = simd::intrinsics::mul<cpx_t>()(x_b, w);
                x_b = simd::intrinsics::sub<cpx_t>()(x_a, w_x_b);
                x_a = simd::intrinsics::add<cpx_t>()(x_a, w_x_b);

                simd::store(&out[j + i], x_a);
                simd::store(&out[j + i + stride], x_b);
            }
        }
    }

    void stage_2n(gsl::span<cpx_t, N> out, int_t stage) const noexcept {
        auto const stride = 1 << stage;

        for (auto j = 0; j < stride; j += simd::width<cpx_t>) {
            auto w1 = simd::load(&w_twiddles[j]);
            auto w2 = simd::intrinsics::mul<cpx_t>()(w1, w1);
            auto w3 = simd::intrinsics::mul<cpx_t>()(w1, w2);

            for (auto i = 0; i < N; i += 4*stride) {
                auto x_a = simd::load(&out[j + i + 0 * stride]);
                auto x_b = simd::load(&out[j + i + 1 * stride]);
                auto x_c = simd::load(&out[j + i + 2 * stride]);
                auto x_d = simd::load(&out[j + i + 3 * stride]);

                auto w_x_b = simd::intrinsics::mul<cpx_t>()(x_b, w1);
                auto w_x_c = simd::intrinsics::mul<cpx_t>()(x_c, w2);
                auto w_x_d = simd::intrinsics::mul<cpx_t>()(x_d, w3);

                x_d = simd::intrinsics::sub<cpx_t>()(w_x_c, w_x_d);
                x_c = simd::intrinsics::add<cpx_t>()(w_x_c, w_x_d);
                x_b = simd::intrinsics::sub<cpx_t>()(x_a, w_x_b);
                x_a = simd::intrinsics::add<cpx_t>()(x_a, w_x_b);

                w_x_d = x_d;
                x_d = simd::intrinsics::sub<cpx_t>()(x_b, w_x_d);
                x_b = simd::intrinsics::add<cpx_t>()(x_b, w_x_d);

                w_x_c = x_c;
                x_c = simd::intrinsics::sub<cpx_t>()(x_a, w_x_c);
                x_a = simd::intrinsics::add<cpx_t>()(x_a, w_x_c);

                simd::store(&out[j + i + 0 * stride], x_a);
                simd::store(&out[j + i + 1 * stride], x_b);
                simd::store(&out[j + i + 2 * stride], x_c);
                simd::store(&out[j + i + 3 * stride], x_d);
            }
        }
    }

    void stage3(gsl::span<cpx_t, N> out) const noexcept {
        for (auto j = 0u; j < 8; j += simd::width<cpx_t>) {
            auto w = simd::load(&w_twiddles[j]);
            for (auto i = 0u; i < N; i += 16) {
                auto x_a = simd::load(&out[j + i + 0]);
                auto x_b = simd::load(&out[j + i + 8]);
                auto w_x_b = simd::multiply(x_b, w);
                x_a = simd::add(x_a, w_x_b);
                x_b = simd::subtract(x_a, w_x_b);

                simd::store(&out[j + i + 0], x_a);
                simd::store(&out[j + i + 8], x_b);
            }
        }
    }


    void stage2(gsl::span<cpx_t, N> out) const noexcept {
        for (auto j = 0u; j < 4; j += simd::width<cpx_t>) {
            auto w = simd::load(&w_twiddles[j]);
            for (auto i = 0u; i < N; i += 8) {
                auto x_a = simd::load(&out[j + i + 0]);
                auto x_b = simd::load(&out[j + i + 4]);
                auto w_x_b = simd::multiply(x_b, w);
                x_a = simd::add(x_a, w_x_b);
                x_b = simd::subtract(x_a, w_x_b);

                simd::store(&out[j + i + 0], x_a);
                simd::store(&out[j + i + 4], x_b);
            }
        }
    }

    void stage1(gsl::span<cpx_t, N> out) const noexcept {
        for (auto i = 0u; i < N; i += 4) {
            auto x_a = out[i];
            auto x_b = out[i + 1];
            auto x_c = out[i + 2];
            auto x_d = flip<direction::forward>(out[i + 3]);

            out[i] = x_a + x_c;
            out[i + 1] = x_b + x_d;
            out[i + 2] = x_a - x_c;
            out[i + 3] = x_b - x_d;
        }
    }

    void stage0(gsl::span<cpx_t, N> out) const noexcept {
        for (auto i = 0u; i < N; i += 2) {
            auto t = out[i + 1];
            out[i + 1] = out[i] - t;
            out[i] += t;
        }
    }

    std::array<cpx_t, N/2> twiddles;
    std::array<cpx_t, N/2> __attribute__((aligned (16))) w_twiddles;
};

}
}
