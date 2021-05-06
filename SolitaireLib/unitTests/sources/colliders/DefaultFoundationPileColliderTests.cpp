#include "colliders/DefaultFoundationPileCollider.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::geometry;

namespace solitaire::colliders {

TEST(DefaultFoundationPileColliderTests, getPosition) {
    Position position {2, 5};
    DefaultFoundationPileCollider collider {position};
    EXPECT_EQ(collider.getPosition(), position);
}

}
