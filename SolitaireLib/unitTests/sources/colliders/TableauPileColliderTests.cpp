#include "cards/Card.h"
#include "colliders/TableauPileCollider.h"
#include "gtest/gtest.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::colliders {

namespace {
using TopCoveredCardPosition = unsigned;
using Index = unsigned;

constexpr int coveredCardsSpacing {3};
constexpr int uncoveredCardsSpacing {16};

const Cards noCards;
const Cards cards {5};
constexpr Position pilePosition {16, 144};
}

class TableauPileColliderTests: public Test {
public:
    TableauPileMock tableauPileMock;
    TableauPileCollider collider {pilePosition, tableauPileMock};
};

TEST_F(TableauPileColliderTests, throwOnGetCardPositionWhenTableauPileIsEmpty) {
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(noCards));
    EXPECT_THROW(collider.getCardPosition(0), std::runtime_error);
}

TEST_F(TableauPileColliderTests, throwOnGetCardPositionWhenInvalidTableauPileCardIndex) {
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(cards));
    EXPECT_THROW(collider.getCardPosition(5), std::runtime_error);
}

struct CardPositionData {
    TopCoveredCardPosition topCoveredCardPosition;
    Index index;
    Position pilePosition;
};

class TableauPileColliderCardPositionTests:
    public TableauPileColliderTests, public WithParamInterface<CardPositionData>
{
};

TEST_P(TableauPileColliderCardPositionTests, getCardPosition) {
    const auto& cardPositionData = GetParam();
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(cards));
    EXPECT_CALL(tableauPileMock, getTopCoveredCardPosition())
        .WillOnce(Return(cardPositionData.topCoveredCardPosition));

    EXPECT_EQ(
        collider.getCardPosition(cardPositionData.index),
        cardPositionData.pilePosition
    );
}

INSTANTIATE_TEST_SUITE_P(CardPositions, TableauPileColliderCardPositionTests, Values(
    CardPositionData {TopCoveredCardPosition {0}, Index {0}, pilePosition},
    CardPositionData {TopCoveredCardPosition {0}, Index {1},
                      pilePosition + Position {0, uncoveredCardsSpacing}},
    CardPositionData {TopCoveredCardPosition {0}, Index {4},
                      pilePosition + Position {0, 4 * uncoveredCardsSpacing}},
    CardPositionData {TopCoveredCardPosition {5}, Index {0}, pilePosition},
    CardPositionData {TopCoveredCardPosition {5}, Index {1},
                      pilePosition + Position {0, coveredCardsSpacing}},
    CardPositionData {TopCoveredCardPosition {5}, Index {4},
                      pilePosition + Position {0, 4 * coveredCardsSpacing}},
    CardPositionData {TopCoveredCardPosition {2}, Index {2},
                      pilePosition + Position {0, 2 * coveredCardsSpacing}},
    CardPositionData {TopCoveredCardPosition {2}, Index {4}, pilePosition +
                      Position {0, 2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing}}
));

}
