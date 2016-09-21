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

#include <cassert>
#include <numeric>
#include <gsl/span>

#include <re/lib/common.hpp>

namespace re {
namespace math {


template <typename T, int_t N>
T
mean(gsl::span<T const, N> input)
noexcept
{
    assert(std::size(input) > 0);
    return std::accumulate(
        std::cbegin(input),
        std::cend(input),
        static_cast<T>(0)
    ) / std::size(input);
}

}
}
