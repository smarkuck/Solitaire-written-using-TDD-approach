#include "geometry/Position.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::geometry {

TEST(PositionTests, equalOperator) {
    Position position1 {3, 7};
    Position position2 {3, 8};
    Position position3 {4, 7};

    EXPECT_TRUE(position1 == position1);
    EXPECT_FALSE(position1 == position2);
    EXPECT_FALSE(position1 == position3);
    EXPECT_FALSE(position2 == position3);
}

TEST(PositionTests, minusOperator) {
    Position minuend {1, 2};
    Position subtrahend {4, 6};
    Position difference {-3, -4};
    EXPECT_EQ(minuend - subtrahend, difference);
}

TEST(PositionTests, plusOperator) {
    Position addend1 {1, 2};
    Position addend2 {4, 6};
    Position sum {5, 8};
    EXPECT_EQ(addend1 + addend2, sum);
}

}
