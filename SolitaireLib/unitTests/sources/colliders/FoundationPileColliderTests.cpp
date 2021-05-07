#include "colliders/FoundationPileCollider.h"
#include "geometry/Size.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::geometry;

namespace solitaire::colliders {

namespace {
constexpr Size cardSize {75, 104};

constexpr Position topLeftCorner {40, 50};

constexpr Position bottomRightCorner {
    topLeftCorner.x + cardSize.width - 1,
    topLeftCorner.y + cardSize.height - 1
};
}

class FoundationPileColliderTests: public Test {
public:
    FoundationPileCollider collider {topLeftCorner};
};

TEST_F(FoundationPileColliderTests, getPosition) {
    EXPECT_EQ(collider.getPosition(), topLeftCorner);
}

TEST_F(FoundationPileColliderTests, collidesWithPoint) {
    EXPECT_FALSE(collider.collidesWithPoint(topLeftCorner - Position {1, 0}));
    EXPECT_FALSE(collider.collidesWithPoint(topLeftCorner - Position {0, 1}));
    EXPECT_FALSE(collider.collidesWithPoint(bottomRightCorner + Position {1, 0}));
    EXPECT_FALSE(collider.collidesWithPoint(bottomRightCorner + Position {0, 1}));
    EXPECT_TRUE(collider.collidesWithPoint(topLeftCorner));
    EXPECT_TRUE(collider.collidesWithPoint(bottomRightCorner));
}

TEST_F(FoundationPileColliderTests, collidesWithCard) {
    EXPECT_FALSE(collider.collidesWithCard(topLeftCorner - Position {cardSize.width, 0}));
    EXPECT_FALSE(collider.collidesWithCard(topLeftCorner - Position {0, cardSize.height}));
    EXPECT_FALSE(collider.collidesWithCard(topLeftCorner + Position {cardSize.width, 0}));
    EXPECT_FALSE(collider.collidesWithCard(topLeftCorner + Position {0, cardSize.height}));

    EXPECT_TRUE(collider.collidesWithCard(
        topLeftCorner - Position {cardSize.width - 1, cardSize.height - 1}));
    EXPECT_TRUE(collider.collidesWithCard(bottomRightCorner));
}

}
