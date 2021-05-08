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
using IsCollided = bool;
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

INSTANTIATE_TEST_SUITE_P(
CardPositions, TableauPileColliderCardPositionTests,
Values(
// uncovered cards
    CardPositionData {
        TopCoveredCardPosition {0},
        Index {0},
        pilePosition
    },

    CardPositionData {
        TopCoveredCardPosition {0},
        Index {1},
        pilePosition + Position {0, uncoveredCardsSpacing}
    },

    CardPositionData {
        TopCoveredCardPosition {0},
        Index {4},
        pilePosition + Position {0, uncoveredCardsSpacing * 4}
    },
// covered cards
    CardPositionData {
        TopCoveredCardPosition {5},
        Index {0},
        pilePosition
    },

    CardPositionData {
        TopCoveredCardPosition {5},
        Index {1},
        pilePosition + Position {0, coveredCardsSpacing}
    },

    CardPositionData {
        TopCoveredCardPosition {5},
        Index {4},
        pilePosition + Position {0, coveredCardsSpacing * 4}
    },
// five cards with two covered
    CardPositionData {
        TopCoveredCardPosition {2},
        Index {2},
        pilePosition + Position {0, coveredCardsSpacing * 2}
    },

    CardPositionData {
        TopCoveredCardPosition {2},
        Index {4},
        pilePosition + Position {0, coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2}
    }
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

INSTANTIATE_TEST_SUITE_P(
BorderPoints, TableauPileColliderCollidedCardTests,
Values(
// empty pile
    CollidedCardData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        middlePilePosition,
        CollidedCardIndex {std::nullopt}
    },
// x position tests
    CollidedCardData {
        CardsQuantity {1},
        TopCoveredCardPosition {0},
        middleYPilePosition + Position {-1, 0},
        CollidedCardIndex {std::nullopt}
    },

    CollidedCardData {
        CardsQuantity {1},
        TopCoveredCardPosition {0},
        middleYPilePosition + Position {cardSize.width, 0},
        CollidedCardIndex {std::nullopt}
    },

    CollidedCardData {
        CardsQuantity {1},
        TopCoveredCardPosition {0},
        middleYPilePosition,
        CollidedCardIndex {0}},

    CollidedCardData {
        CardsQuantity {1},
        TopCoveredCardPosition {0},
        middleYPilePosition + Position {cardSize.width - 1, 0},
        CollidedCardIndex {0}
    },
// y position tests
    CollidedCardData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        middleXPilePosition + Position {0, -1},
        CollidedCardIndex {std::nullopt}
    },

    CollidedCardData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        middleXPilePosition +
        Position {0, coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 - 1},
        CollidedCardIndex {3}
    },

    CollidedCardData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        middleXPilePosition +
        Position {0, coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2},
        CollidedCardIndex {4}
    },

    CollidedCardData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        middleXPilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 + cardSize.height - 1},
        CollidedCardIndex {4}},

    CollidedCardData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        middleXPilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 + cardSize.height},
        CollidedCardIndex {std::nullopt}}
));

struct CollidedCardsInHandData {
    CardsQuantity cardsQuantity;
    TopCoveredCardPosition topCoveredCardPosition;
    Position cardsInHandPosition;
    IsCollided isCollided;
};

class TableauPileColliderCollideWithCardsInHandTests:
    public TableauPileColliderTests,
    public WithParamInterface<CollidedCardsInHandData>
{
};

TEST_P(TableauPileColliderCollideWithCardsInHandTests, collidesWithCardsInHand) {
    const auto& collidedCardsInHandData = GetParam();
    Cards cards {collidedCardsInHandData.cardsQuantity};

    EXPECT_CALL(tableauPileMock, getCards()).WillRepeatedly(ReturnRef(cards));
    EXPECT_CALL(tableauPileMock, getTopCoveredCardPosition())
        .WillRepeatedly(Return(collidedCardsInHandData.topCoveredCardPosition));

    EXPECT_EQ(
        collider.collidesWithCardsInHand(collidedCardsInHandData.cardsInHandPosition),
        collidedCardsInHandData.isCollided
    );
}

INSTANTIATE_TEST_SUITE_P(
BorderPoints, TableauPileColliderCollideWithCardsInHandTests,
Values(
// x position tests
    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition - Position {cardSize.width, 0},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition + Position {cardSize.width, 0},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition - Position {cardSize.width - 1, 0},
        IsCollided {true}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition + Position {cardSize.width - 1, 0},
        IsCollided {true}
    },
// empty pile: y position tests
    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition - Position {0, cardSize.height},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition + Position {0, cardSize.height},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition - Position {0, cardSize.height - 1},
        IsCollided {true}
    },

    CollidedCardsInHandData {
        CardsQuantity {0},
        TopCoveredCardPosition {0},
        pilePosition + Position {0, cardSize.height - 1},
        IsCollided {true}
    },
// pile with cards: y position tests
    CollidedCardsInHandData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        pilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 - cardSize.height},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        pilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 + cardSize.height},
        IsCollided {false}
    },

    CollidedCardsInHandData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        pilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 - cardSize.height + 1},
        IsCollided {true}
    },

    CollidedCardsInHandData {
        CardsQuantity {5},
        TopCoveredCardPosition {2},
        pilePosition + Position {0,
        coveredCardsSpacing * 2 + uncoveredCardsSpacing * 2 + cardSize.height - 1},
        IsCollided {true}
    }
));

}
