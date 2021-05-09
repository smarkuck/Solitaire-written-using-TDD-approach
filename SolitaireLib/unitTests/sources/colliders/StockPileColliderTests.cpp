#include "gmock/gmock.h"

#include "cards/Card.h"
#include "cards/Cards.h"
#include "colliders/StockPileCollider.h"
#include "geometry/Position.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/StockPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::colliders {

namespace {
using CardsQuantity = unsigned;
using SelectedCardIndex = std::optional<unsigned>;
constexpr unsigned pileCardsSpacing {2};
constexpr Position pilePosition {16, 30};
constexpr Position pileBottomRightCorner {90, 133};
constexpr Position pileUncoveredCardsPosition {105, 30};
constexpr Position pileUncoveredCardsBottomRightCorner {179, 133};
}

class StockPileColliderTests: public Test {
public:
    StockPileMock stockPileMock;
    StockPileCollider collider {stockPileMock};
};

struct CoveredPileCardsPositionData {
    CardsQuantity cardsQuantity;
    SelectedCardIndex selectedCardIndex;
    Position position;
};

class StockPileColliderGetCoveredCardsPositionTests:
    public StockPileColliderTests,
    public WithParamInterface<CoveredPileCardsPositionData>
{
};

TEST_P(StockPileColliderGetCoveredCardsPositionTests, getCoveredCardsPosition) {
    const auto& coveredCardsPositionData = GetParam();
    Cards cards {coveredCardsPositionData.cardsQuantity};
    EXPECT_CALL(stockPileMock, getCards()).WillOnce(ReturnRef(cards));
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillOnce(Return(coveredCardsPositionData.selectedCardIndex));

    EXPECT_EQ(
        collider.getCoveredCardsPosition(),
        coveredCardsPositionData.position
    );
}

INSTANTIATE_TEST_SUITE_P(
BorderPositions, StockPileColliderGetCoveredCardsPositionTests,
Values(
    CoveredPileCardsPositionData {
        CardsQuantity {0},
        SelectedCardIndex {std::nullopt},
        pilePosition
    },

    CoveredPileCardsPositionData {
        CardsQuantity {6},
        SelectedCardIndex {std::nullopt},
        pilePosition
    },

    CoveredPileCardsPositionData {
        CardsQuantity {7},
        SelectedCardIndex {std::nullopt},
        pilePosition + Position {pileCardsSpacing, 0}
    },

    CoveredPileCardsPositionData {
        CardsQuantity {12},
        SelectedCardIndex {std::nullopt},
        pilePosition + Position {pileCardsSpacing, 0}
    },

    CoveredPileCardsPositionData {
        CardsQuantity {13},
        SelectedCardIndex {std::nullopt},
        pilePosition + Position {pileCardsSpacing * 2, 0}
    },

    CoveredPileCardsPositionData {
        CardsQuantity {14},
        SelectedCardIndex {1},
        pilePosition + Position {pileCardsSpacing, 0}
    },

    CoveredPileCardsPositionData {
        CardsQuantity {15},
        SelectedCardIndex {1},
        pilePosition + Position {pileCardsSpacing * 2, 0}
    }
));

struct UncoveredPileCardsPositionData {
    SelectedCardIndex selectedCardIndex;
    Position position;
};

class StockPileColliderGetUncoveredCardsPositionTests:
    public StockPileColliderTests,
    public WithParamInterface<UncoveredPileCardsPositionData>
{
};

TEST_P(StockPileColliderGetUncoveredCardsPositionTests, getUncoveredCardsPosition) {
    const auto& uncoveredCardsPositionData = GetParam();
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillOnce(Return(uncoveredCardsPositionData.selectedCardIndex));

    EXPECT_EQ(
        collider.getUncoveredCardsPosition(),
        uncoveredCardsPositionData.position
    );
}

INSTANTIATE_TEST_SUITE_P(
BorderPositions, StockPileColliderGetUncoveredCardsPositionTests,
Values(
    UncoveredPileCardsPositionData {
        SelectedCardIndex {std::nullopt},
        pileUncoveredCardsPosition
    },

    UncoveredPileCardsPositionData {
        SelectedCardIndex {5},
        pileUncoveredCardsPosition
    },

    UncoveredPileCardsPositionData {
        SelectedCardIndex {6},
        pileUncoveredCardsPosition + Position {pileCardsSpacing, 0}
    },

    UncoveredPileCardsPositionData {
        SelectedCardIndex {11},
        pileUncoveredCardsPosition + Position {pileCardsSpacing, 0}
    },

    UncoveredPileCardsPositionData {
        SelectedCardIndex {12},
        pileUncoveredCardsPosition + Position {pileCardsSpacing * 2, 0}
    }
));

TEST_F(StockPileColliderTests, emptyCoveredCardsCollidesWith) {
    Cards noCards;
    EXPECT_CALL(stockPileMock, getCards()).WillRepeatedly(ReturnRef(noCards));
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillRepeatedly(Return(std::nullopt));

    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pilePosition - Position {1, 0}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pilePosition - Position {0, 1}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner + Position {1, 0}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner + Position {0, 1}));
    EXPECT_TRUE(collider.coveredCardsCollidesWith(
        pilePosition));
    EXPECT_TRUE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner));
}

TEST_F(StockPileColliderTests, thirteenCoveredCardsCollidesWith) {
    Cards cards {13};
    EXPECT_CALL(stockPileMock, getCards()).WillRepeatedly(ReturnRef(cards));
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillRepeatedly(Return(std::nullopt));

    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pilePosition + Position {pileCardsSpacing * 2 - 1, 0}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pilePosition + Position {pileCardsSpacing * 2, -1}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner + Position {pileCardsSpacing * 2 + 1, 0}));
    EXPECT_FALSE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner + Position {pileCardsSpacing * 2, 1}));
    EXPECT_TRUE(collider.coveredCardsCollidesWith(
        pilePosition + Position {pileCardsSpacing * 2, 0}));
    EXPECT_TRUE(collider.coveredCardsCollidesWith(
        pileBottomRightCorner + Position {pileCardsSpacing * 2, 0}));
}

TEST_F(StockPileColliderTests, emptyUncoveredCardsDoesNotCollideWith) {
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillRepeatedly(Return(std::nullopt));

    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(pileUncoveredCardsPosition));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(pileBottomRightCorner));
}

TEST_F(StockPileColliderTests, oneUncoveredCardCollidesWith) {
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillRepeatedly(Return(0));

    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition - Position {1, 0}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition - Position {0, 1}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner + Position {1, 0}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner + Position {0, 1}));
    EXPECT_TRUE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition));
    EXPECT_TRUE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner));
}

TEST_F(StockPileColliderTests, thirteenUncoveredCardsCollidesWith) {
    EXPECT_CALL(stockPileMock, getSelectedCardIndex())
        .WillRepeatedly(Return(12));

    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition + Position {pileCardsSpacing * 2 - 1, 0}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition + Position {pileCardsSpacing * 2, -1}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner + Position {pileCardsSpacing * 2 + 1, 0}));
    EXPECT_FALSE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner + Position {pileCardsSpacing * 2, 1}));
    EXPECT_TRUE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsPosition + Position {pileCardsSpacing * 2, 0}));
    EXPECT_TRUE(collider.uncoveredCardsCollidesWith(
        pileUncoveredCardsBottomRightCorner + Position {pileCardsSpacing * 2, 0}));
}

}
