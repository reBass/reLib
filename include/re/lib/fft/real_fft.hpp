// Copyright (c) 2016 Roman Beránek. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <array>
#include <complex>
#include <type_traits>

#include <re/lib/common.hpp>
#include <re/lib/fft/fft.hpp>

namespace re {
namespace fft {

template <typename T, int_t N, direction Direction>
class real_fft
{
    static_assert(std::is_floating_point<T>::value);
    static_assert(N % 4 == 0, "N must be a divisible by 4.");
    using real_t = T;
    using cpx_t = std::complex<T>;

public:
    real_fft() noexcept
    {
        auto const step = (is_inverse(Direction) ? -2 : 2) * pi<real_t> / N;
        for (auto i = 0u; i < std::size(twiddles); ++i) {
            twiddles[i] = std::polar(real_t{1}, (i + N / 4) * step);
        }
    }

    void
    operator()(
        std::conditional_t<
            is_forward(Direction),
            gsl::span<real_t const, N>,
            gsl::span<cpx_t, N/2 + 1>
        > input,
        std::conditional_t<
            is_forward(Direction),
            gsl::span<cpx_t, N/2 + 1>,
            gsl::span<real_t, N>
        > output
    ) const noexcept
    {
        transform(input, output);
    }

private:
    void
    transform(
        gsl::span<real_t const, N> in,
        gsl::span<cpx_t, N/2 + 1> out
    ) const noexcept
    {
        auto cpx_in = gsl::span<cpx_t const, N/2>(
            reinterpret_cast<cpx_t const*>(in.data()),
            N/2
        );
        auto out_span = subspan<0, N/2>(out);

        fft_(cpx_in, out_span);
        real_to_cpx(out);
    }

    void
    transform(
        gsl::span<cpx_t, N/2 + 1> in,
        gsl::span<real_t, N> out
    ) const noexcept
    {
        real_to_cpx(in);
        auto const_in = subspan<0, N/2>(in);
        auto cpx_out = gsl::span<cpx_t, N/2>(
            reinterpret_cast<cpx_t*>(std::data(out)),
            N/2
        );
        fft_(const_in, cpx_out);
    }

    void
    real_to_cpx(gsl::span<cpx_t, N/2 + 1> data) const noexcept
    {
        if (is_inverse(Direction)) {
            data[0] = {
                data[0].real() + data[N/2].real(),
                data[0].real() - data[N/2].real()
            };
        } else {
            data[N/2] = data[0].real() - data[0].imag();
            data[0] = data[0].real() + data[0].imag();
        }

        for (auto i = 1u; 4 * i < N; ++i) {
            auto z = std::conj(data[N/2 - i]);
            auto w = data[i] + z;
            z = multiply_fast(data[i] - z, twiddles[i]);

            data[i] = w + z;
            data[N/2 - i] = std::conj(w - z);

            if (is_forward(Direction)) {
                data[i] = 0.5f * data[i];
                data[N/2 - i] = 0.5f * data[N/2 - i];
            }
        }
        data[N/4] = std::conj(data[N/4]);
        if (is_forward(Direction)) {
            data[N/4] = 0.5f * data[N/4];
        }
    }

    fft<real_t, N/2, Direction> fft_;
    std::array<cpx_t, N/2> twiddles;
};

} // fft
} // re
