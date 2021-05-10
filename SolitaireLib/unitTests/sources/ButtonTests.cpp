#include "Button.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::geometry;

namespace solitaire {

namespace {
constexpr geometry::Position position {34, 23};
constexpr geometry::Position bottomLeftCorner {91, 39};
}

class ButtonTests: public Test {
public:
    Button button {position};
};

TEST_F(ButtonTests, buttonIsNotHoveredInitially) {
    EXPECT_FALSE(button.isHovered());
}

TEST_F(ButtonTests, setAndGetButtonState) {
    button.setHoveredState(true);
    EXPECT_TRUE(button.isHovered());
    button.setHoveredState(false);
    EXPECT_FALSE(button.isHovered());
}

TEST_F(ButtonTests, collidesWith) {
    EXPECT_FALSE(button.collidesWith(position - Position {1, 0}));
    EXPECT_FALSE(button.collidesWith(position - Position {0, 1}));
    EXPECT_FALSE(button.collidesWith(bottomLeftCorner + Position {1, 0}));
    EXPECT_FALSE(button.collidesWith(bottomLeftCorner + Position {0, 1}));
    EXPECT_TRUE(button.collidesWith(position));
    EXPECT_TRUE(button.collidesWith(bottomLeftCorner));
}

}
