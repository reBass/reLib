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

#include <algorithm>
#include <array>
#include <re/lib/common.hpp>

namespace re {
namespace simd {

template <typename T> struct vector_of { using type = T; };
template <typename T> using vec_t = typename vector_of<T>::type;
template <typename T> constexpr int width = sizeof(vec_t<T>) / sizeof(T);

template <typename T>
union lane {
    std::array<T, width<T>> a;
    vec_t<T> v;

    lane()
    {
    }

    lane(vec_t<T> rhs) :
        v(rhs)
    {
    }

    operator vec_t<T>&() {
        return v;
    }
};

template <typename T, typename BinaryOp>
lane<T>& lane_transform(lane<T> const& a, lane<T> const& b, lane<T>& output, BinaryOp op) {
    std::transform(cbegin(a.a), cend(a.a), cbegin(a.a), begin(output.a), op);
    return output;
};

template <typename T, typename UnaryOp>
lane<T>& lane_transform(lane<T> const& a, lane<T>& output, UnaryOp op) {
    std::transform(cbegin(a.a), cend(a.a), begin(output.a), op);
    return output;
};


namespace intrinsics {

template <typename T> struct zero {
    lane<T> operator()() {
        return lane<T>();
    }
};
template <typename T> struct load {
    lane<T> operator()(T const* p) {
        lane<T> v;
        std::copy(p, p + width<T>, begin(v.a));
        return v;
    }
};
template <typename T> struct hadd {
    constexpr T operator()(T a, T b) { return a; }
    lane<T> operator()(lane<T> a, lane<T> b) {
        auto const half = width<T>;
        for (auto i = 0; i < half; ++i) {
            a.a[i] = a.a[2*i] + a.a[2*i + 1];
        }
        for (auto i = 0; i < half; ++i) {
            a.a[i + half] = b.a[2*i] + b.a[2*i + 1];
        }
        return a;
    }
};
template <typename T> struct add {
    constexpr T operator()(T a, T b) { return a + b; }
    lane<T> operator()(lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, add<T>());
    }
    T operator<<(lane<T> a) {
        for (auto i = width<T>; i > 1; i >>= 2) {
            a = hadd<T>()(a, a);
        }
        return a.a[0];
    }
};
template <typename T> struct sub {
    constexpr T operator()(T a, T b) { return a - b; }
    lane<T> operator()(lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, sub<T>());
    }
};
template <typename T> struct mul {
    constexpr T operator()(T a, T b) { return a * b; }
    lane<T> operator()(lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, mul<T>());
    }
};
template <typename T> struct sqr {
    constexpr T operator()(T a) { return a * a; }
    lane<T> operator()(lane<T> a) {
        return lane_transform(a, a, sqr<T>());
    }
};
template <typename T> struct sqrt {
    constexpr T operator()(T a) { return std::sqrt(a); }
    lane<T> operator()(lane<T> a) {
        return lane_transform(a, a, sqrt<T>());
    }
};

}

/*
template <typename T>
struct intrinsics {
    static constexpr auto load = [](T const* data) { return *data; };
    static constexpr auto add = [](T a, T b) { return a + b; };
    static constexpr auto subtract = [](T a, T b) { return a - b; };
    static constexpr auto multiply = [](T a, T b) { return a * b; };
    static constexpr auto square = [](T a) { return multiply(a, a); };
    static constexpr auto square_root = [](T a) { return std::sqrt(a); };
};

template <typename T>
intrinsics<lane<T>>::load = [](T const* data) {
    lane<T> v;
    copy(data, data + width<T>, begin(v.a));
    return v;
};

template <typename T>
intrinsics<lane<T>>::add = [](lane<T> a, lane<T> b) {
    return lane_transform(a, b, a, intrinsics<T>::add);
};
/*
template <typename T>
struct intrinsics<lane<T>> {
    static constexpr auto subtract = [](lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, intrinsics<T>::subtract);
    };
    static constexpr auto multiply = [](lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, intrinsics<T>::multiply);
    };

    /*
    static lane<T> square(lane<T> v) {
        return multiply(v, v);
    }
    static lane<T> square_root(lane<T> v) {
        transform(cbegin(v.a), cend(v.a), begin(v.a), [](auto x) { return sqrt(x); });
        return v;
    }

    static constexpr auto add_horizontally = [](lane<T> a, lane<T> b) {
        auto const half = width<T>;
        for (auto i = 0; i < half; ++i) {
            a.a[i] = a.a[2*i] + a.a[2*i + 1];
        }
        for (auto i = 0; i < half; ++i) {
            a.a[i + half] = b.a[2*i] + b.a[2*i + 1];
        }
        return a;
    };

    static constexpr auto zero = []() { return lane<T>(); };
};
*/
}
}
