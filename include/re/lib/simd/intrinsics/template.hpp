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
#include <bitset>
#include <cmath>
#include <numeric>
#include <functional>

#include <re/lib/common.hpp>

namespace re {
namespace simd {

template <typename T> struct vector_of { using type = T; };
template <typename T> using vec_t = typename vector_of<T>::type;
template <typename T> constexpr int width = sizeof(vec_t<T>) / sizeof(T);

template <typename T> struct lane_ptr {
    T* ptr;
    explicit lane_ptr(T* p) : ptr{p} {}
    operator T*() { return ptr; }
};

template <typename T>
union lane {
    std::array<T, width<T>> a;
    vec_t<T> v;

    lane() { }
    lane(vec_t<T> rhs) : v(rhs) { }
    operator vec_t<T>&() { return v; }
};

template <typename T, typename BinaryOp>
lane<T>& lane_transform(
    lane<T> const& a,
    lane<T> const& b,
    lane<T>& output,
    BinaryOp op
) {
    std::transform(
        std::cbegin(a.a),
        std::cend(a.a),
        std::cbegin(b.a),
        std::begin(output.a),
        op
    );
    return output;
};

template <typename T, typename UnaryOp>
lane<T>& lane_transform(lane<T> const& a, lane<T>& output, UnaryOp op) {
    std::transform(
        std::cbegin(a.a),
        std::cend(a.a),
        std::begin(output.a),
        op
    );
    return output;
};

template <typename T, typename BinaryOp>
T lane_accumulate(lane<T> const& a, T initial, BinaryOp op) {
    return std::accumulate(
        std::cbegin(a.a),
        std::cend(a.a),
        initial,
        op
    );
};


namespace intrinsics {

template <typename T> lane<T> zero() {
    lane<T> v;
    for (auto& a : v.a) {
        a = static_cast<T>(0);
    }
    return v;
}

template <typename T> struct set {
    lane<T> operator()(T value) {
        lane <T> v;
        for (auto& a : v.a) {
            a = value;
        }
        return v;
    }
};
template <typename T> struct load {
    T operator()(T const* p) {
        return *p;
    }
    lane<T> operator()(lane_ptr<T const> p) {
        lane<T> v;
        std::copy(p.ptr, p.ptr + width<T>, begin(v.a));
        return v;
    }
};
template <typename T> struct store {
    void operator()(T* p, T value) {
        *p = value;
    }
    void operator()(lane_ptr<T> p, lane<T> value) {
        std::copy(std::begin(value.a), std::end(value.a), p.ptr);
    }
};
template <typename T> struct fill {
    lane<T> operator()(T value) {
        lane<T> v;
        v.a.fill(value);
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
    T operator()(lane<T> a) {
        return lane_accumulate(a, static_cast<T>(0), add<T>());
    }
    T operator<<(lane<T> a) {
        return lane_accumulate(a, static_cast<T>(0), add<T>());
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
template <typename T> struct reduce_add {
    T operator()(lane<T> a) {
        return lane_accumulate(a, static_cast<T>(0), std::plus<T>());
    }
};

template <typename T> struct max {
    T operator()(T a, T b) {
        return std::fmax(a, b);
    }
    lane<T> operator()(lane<T> a, lane<T> b) {
        return lane_transform(a, b, a, *this);
    }
};

template <typename T> struct any_greater_than {
    bool operator()(T a, T b) {
        return a > b;
    }
    bool operator()(lane<T> a, lane<T> b) {
        return lane_accumulate(
            sub<T>()(a, b),
            false,
            [](bool r, T d) {
                return r || d > 0;
            }
        );
    }
};

}


template <typename T>
inline lane<T> load(T const* p) {
    return intrinsics::load<T>()(lane_ptr<T const>(p));
}

template <typename T>
inline void store(T* p, lane<T> value) {
    intrinsics::store<T>()(lane_ptr<T>(p), value);
}


template <typename T>
inline lane<T> add(lane<T> a, lane<T> b) {
    return intrinsics::add<T>()(a, b);
}

template <typename T>
inline lane<T> subtract(lane<T> a, lane<T> b) {
    return intrinsics::sub<T>()(a, b);
}

template <typename T>
inline lane<T> multiply(lane<T> a, lane<T> b) {
    return intrinsics::mul<T>()(a, b);
}

template <typename T>
inline lane<T> set_lane(T value) {
    return intrinsics::set<T>()(value);
}



}
}
