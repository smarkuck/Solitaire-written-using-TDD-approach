#include "Layout.h"
#include "gtest/gtest.h"
#include "piles/PileId.h"

using namespace testing;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire {

TEST(LayoutTests, getFoundationPilePosition) {
    Position pilePosition1 {283, 30};
    Position pilePosition2 {372, 30};
    Position pilePosition3 {461, 30};
    Position pilePosition4 {550, 30};

    EXPECT_EQ(Layout::getFoundationPilePosition(PileId {0}), pilePosition1);
    EXPECT_EQ(Layout::getFoundationPilePosition(PileId {1}), pilePosition2);
    EXPECT_EQ(Layout::getFoundationPilePosition(PileId {2}), pilePosition3);
    EXPECT_EQ(Layout::getFoundationPilePosition(PileId {3}), pilePosition4);
    EXPECT_THROW(Layout::getFoundationPilePosition(PileId {4}), std::runtime_error);
}

TEST(LayoutTests, getTableauPilePosition) {
    Position pilePosition1 {16, 144};
    Position pilePosition2 {105, 144};
    Position pilePosition3 {194, 144};
    Position pilePosition4 {283, 144};
    Position pilePosition5 {372, 144};
    Position pilePosition6 {461, 144};
    Position pilePosition7 {550, 144};

    EXPECT_EQ(Layout::getTableauPilePosition(PileId {0}), pilePosition1);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {1}), pilePosition2);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {2}), pilePosition3);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {3}), pilePosition4);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {4}), pilePosition5);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {5}), pilePosition6);
    EXPECT_EQ(Layout::getTableauPilePosition(PileId {6}), pilePosition7);
    EXPECT_THROW(Layout::getTableauPilePosition(PileId {7}), std::runtime_error);
}

}
