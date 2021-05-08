#include "cards/Card.h"
#include "colliders/TableauPileCollider.h"
#include "geometry/Size.h"
#include "gmock/gmock.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::colliders {

namespace {
using CardsQuantity = unsigned;
using TopCoveredCardPosition = unsigned;
using Index = unsigned;
using CollidedCardIndex = std::optional<unsigned>;

constexpr int coveredCardsSpacing {3};
constexpr int uncoveredCardsSpacing {16};

const Cards noCards;
const Cards cards {5};

constexpr Size cardSize {75, 104};

constexpr Position pilePosition {16, 144};

constexpr Position middleXPilePosition {
    pilePosition.x + cardSize.width/2,
    pilePosition.y
};

constexpr Position middleYPilePosition {
    pilePosition.x,
    pilePosition.y + cardSize.height/2
};

constexpr Position middlePilePosition {
    pilePosition.x + cardSize.width/2,
    pilePosition.y + cardSize.height/2};
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
    Position cardPosition;
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
        cardPositionData.cardPosition
    );
}

INSTANTIATE_TEST_SUITE_P(CardPositions, TableauPileColliderCardPositionTests, Values(
// uncovered cards
    CardPositionData {TopCoveredCardPosition {0}, Index {0}, pilePosition},

    CardPositionData {TopCoveredCardPosition {0}, Index {1},
                      pilePosition + Position {0, uncoveredCardsSpacing}},

    CardPositionData {TopCoveredCardPosition {0}, Index {4},
                      pilePosition + Position {0, 4 * uncoveredCardsSpacing}},
// covered cards
    CardPositionData {TopCoveredCardPosition {5}, Index {0}, pilePosition},

    CardPositionData {TopCoveredCardPosition {5}, Index {1},
                      pilePosition + Position {0, coveredCardsSpacing}},

    CardPositionData {TopCoveredCardPosition {5}, Index {4},
                      pilePosition + Position {0, 4 * coveredCardsSpacing}},
// five cards with two covered
    CardPositionData {TopCoveredCardPosition {2}, Index {2},
                      pilePosition + Position {0, 2 * coveredCardsSpacing}},

    CardPositionData {TopCoveredCardPosition {2}, Index {4}, pilePosition +
                      Position {0, 2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing}}
));

struct CollidedCardData {
    CardsQuantity cardsQuantity;
    TopCoveredCardPosition topCoveredCardPosition;
    Position collidedPosition;
    CollidedCardIndex cardIndex;
};

class TableauPileColliderCollidedCardTests:
    public TableauPileColliderTests, public WithParamInterface<CollidedCardData>
{
};

TEST_P(TableauPileColliderCollidedCardTests, getCollidedCardIndex) {
    const auto& collidedCardData = GetParam();
    Cards cards {collidedCardData.cardsQuantity};

    EXPECT_CALL(tableauPileMock, getCards()).WillRepeatedly(ReturnRef(cards));
    EXPECT_CALL(tableauPileMock, getTopCoveredCardPosition())
        .WillRepeatedly(Return(collidedCardData.topCoveredCardPosition));

    EXPECT_EQ(
        collider.tryGetCollidedCardIndex(collidedCardData.collidedPosition),
        collidedCardData.cardIndex
    );
}

INSTANTIATE_TEST_SUITE_P(BorderPoints, TableauPileColliderCollidedCardTests, Values(
// empty pile
    CollidedCardData {CardsQuantity {0}, TopCoveredCardPosition {0},
                      middlePilePosition, CollidedCardIndex {std::nullopt}},
// one card: x position tests
    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleYPilePosition + Position {-1, 0},
                      CollidedCardIndex {std::nullopt}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleYPilePosition + Position {cardSize.width, 0},
                      CollidedCardIndex {std::nullopt}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleYPilePosition, CollidedCardIndex {0}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleYPilePosition + Position {cardSize.width - 1, 0},
                      CollidedCardIndex {0}},
// one card: y position tests
    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleXPilePosition + Position {0, -1},
                      CollidedCardIndex {std::nullopt}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleXPilePosition + Position {0, cardSize.height},
                      CollidedCardIndex {std::nullopt}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleXPilePosition, CollidedCardIndex {0}},

    CollidedCardData {CardsQuantity {1}, TopCoveredCardPosition {0},
                      middleXPilePosition + Position {0, cardSize.height - 1},
                      CollidedCardIndex {0}},
// five cards: y position tests
    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition, CollidedCardIndex {0}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0, coveredCardsSpacing - 1},
                      CollidedCardIndex {0}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0, coveredCardsSpacing},
                      CollidedCardIndex {1}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0, 2 * coveredCardsSpacing - 1},
                      CollidedCardIndex {1}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0, 2 * coveredCardsSpacing},
                      CollidedCardIndex {2}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition +
                      Position {0, 2 * coveredCardsSpacing + uncoveredCardsSpacing - 1},
                      CollidedCardIndex {2}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition +
                      Position {0, 2 * coveredCardsSpacing + uncoveredCardsSpacing},
                      CollidedCardIndex {3}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition +
                      Position {0, 2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing - 1},
                      CollidedCardIndex {3}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition +
                      Position {0, 2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing},
                      CollidedCardIndex {4}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0,
                      2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing + cardSize.height - 1},
                      CollidedCardIndex {4}},

    CollidedCardData {CardsQuantity {5}, TopCoveredCardPosition {2},
                      middleXPilePosition + Position {0,
                      2 * coveredCardsSpacing + 2 * uncoveredCardsSpacing + cardSize.height},
                      CollidedCardIndex {std::nullopt}}
));

}
