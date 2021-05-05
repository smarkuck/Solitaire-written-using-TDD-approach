#include "geometry/Area.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::geometry {

TEST(AreaTests, equalOperator) {
    Position position1 {3, 8};
    Position position2 {4, 7};

    Size size1 {3, 8};
    Size size2 {4, 7};

    Area area1 {position1, size1};
    Area area2 {position1, size2};
    Area area3 {position2, size1};

    EXPECT_TRUE(area1 == area1);
    EXPECT_FALSE(area1 == area2);
    EXPECT_FALSE(area1 == area3);
    EXPECT_FALSE(area2 == area3);
}

}
