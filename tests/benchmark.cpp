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

#include <array>
#include <cmath>
#include <complex>
#include <numeric>
#include <vector>

#include <gsl/span>

#include <re/lib/revolver.hpp>
#include <re/lib/math/reductions.hpp>

#include <benchmark/benchmark.h>

using namespace re;
using namespace re::math;

static float sin_n(int_t i, int_t n) {
    return std::sinf(static_cast<float>(M_2_PI) * i / n);
}

template <int_t N>
static void fill_sin(gsl::span<float, N> span) {
    for (auto i = 0; i < N; ++i) {
        span[i] = sin_n(i, N);
    }
}

template <int_t N>
static void fill_sin(gsl::span<double, N> span) {
    for (auto i = 0; i < N; ++i) {
        span[i] = static_cast<double>(sin_n(i, N));
    }
}

template <int_t N>
static void fill_sin(gsl::span<long double, N> span) {
    for (auto i = 0; i < N; ++i) {
        span[i] = static_cast<long double>(sin_n(i, N));
    }
}


static void std_mean_1024f(benchmark::State& state) {
    std::array<float, 1024> input;
    fill_sin(gsl::span<float, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = std::accumulate(
            std::cbegin(input),
            std::cend(input),
            0.f
        ) / 1024;
    }
}
BENCHMARK(std_mean_1024f);

static void mean_1024f(benchmark::State& state) {
    std::array<float, 1024> input;
    fill_sin(gsl::span<float, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = mean(gsl::span<float const, 1024>(input));
    }
}
BENCHMARK(mean_1024f);

static void sum_of_squares_1024f(benchmark::State& state) {
    std::array<float, 1024> input;
    fill_sin(gsl::span<float, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = sum_of_squares(gsl::span<float const, 1024>(input));
    }
}
BENCHMARK(sum_of_squares_1024f);

static void std_mean_1024d(benchmark::State& state) {
    std::array<double, 1024> input;
    fill_sin(gsl::span<double, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = std::accumulate(
            std::cbegin(input),
            std::cend(input),
            0.
        ) / 1024;
    }
}
BENCHMARK(std_mean_1024d);

static void mean_1024d(benchmark::State& state) {
    std::array<double, 1024> input;
    fill_sin(gsl::span<double, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = mean(gsl::span<double const, 1024>(input));
    }
}
BENCHMARK(mean_1024d);

static void mean_1024ld(benchmark::State& state) {
    std::array<long double, 1024> input;
    fill_sin(gsl::span<long double, 1024>(input));
    while (state.KeepRunning()) {
        input[0] = mean(gsl::span<long double const, 1024>(input));
    }
}
BENCHMARK(mean_1024ld);

/*
static void stuff() {
    revolver<std::array<float, 8>> ring;
    revolver<std::vector<float>> vector_ring({7});

    vector_ring = {1.0f, 2.3f, 56.f};

    //revolver<std::vector<float>> ring_copy = vector_ring;
    revolver<std::vector<float>> another_copy(vector_ring);
}
*/

BENCHMARK_MAIN();
