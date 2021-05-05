#include "geometry/Size.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::geometry {

TEST(SizeTests, equalOperator) {
    Size size1 {3, 7};
    Size size2 {3, 8};
    Size size3 {4, 7};

    EXPECT_TRUE(size1 == size1);
    EXPECT_FALSE(size1 == size2);
    EXPECT_FALSE(size1 == size3);
    EXPECT_FALSE(size2 == size3);
}

}
