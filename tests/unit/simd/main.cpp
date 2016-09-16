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

#include <gtest/gtest.h>

#include <gsl/span>
#include <re/lib/math/reductions.hpp>

namespace re {
using namespace simd;

class SimdTest : public ::testing::Test {
protected:
    SimdTest() {

    }

    virtual ~SimdTest () {

    }

    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(SimdTest, Whatever) {
    std::array<float, 5> input;
    gsl::span<float, 5> input_span(input);

    EXPECT_TRUE(true);
}

} // namespace re

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
