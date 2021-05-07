#include "archivers/SnapshotMock.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/PileUtils.h"
#include "piles/StockPile.h"

using namespace testing;
using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(StockPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    StockPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyStockPileTest: public Test {
public:
    std::shared_ptr<StockPile> pile {std::make_shared<StockPile>()};
};

TEST_F(EmptyStockPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(EmptyStockPileTest, trySelectNextCard) {
    pile->trySelectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

class StockPileWithCardsTest: public EmptyStockPileTest {
public:
    StockPileWithCardsTest() {
        initializePile(*pile, pileCards);
    }

    Cards pileCards {
        Card {Value::Ten, Suit::Heart},
        Card {Value::Three, Suit::Spade},
        Card {Value::Two, Suit::Diamond}
    };
};

TEST_F(StockPileWithCardsTest, initializePileWithCards) {
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, trySelectNextCard) {
    pile->trySelectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);

    pile->trySelectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 1);

    pile->trySelectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 2);

    pile->trySelectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutCardWhenNoneIsSelected) {
    EXPECT_EQ(pile->tryPullOutCard(), std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutFirstCard) {
    const auto firstCard = pileCards.front();
    pileCards.erase(pileCards.begin());

    pile->trySelectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), firstCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutSecondCard) {
    const auto secondCardIt = std::next(pileCards.begin());
    const auto secondCard = *secondCardIt;
    pileCards.erase(secondCardIt);

    pile->trySelectNextCard();
    pile->trySelectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), secondCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);
}

TEST_F(StockPileWithCardsTest, tryPullOutLastCard) {
    const Card lastCard = pileCards.back();
    pileCards.pop_back();

    pile->trySelectNextCard();
    pile->trySelectNextCard();
    pile->trySelectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), lastCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 1);
}

class StockPileInitializationTest: public StockPileWithCardsTest {
public:
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };
};

TEST_F(StockPileInitializationTest, initializePileAfterOperations) {
    initializePile(*pile, newPileCards);

    EXPECT_THAT(pile->getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileInitializationTest, restorePileStateUsingSnapshot) {
    pile->trySelectNextCard();
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);
}

TEST_F(StockPileInitializationTest, isSnapshotOfSameObject) {
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    const auto snapshotOfSameObject = pile->createSnapshot();
    const auto snapshotOfSameTypeObject = std::make_shared<StockPile>()->createSnapshot();
    SnapshotMock snapshotOfDifferentTypeObject;

    EXPECT_TRUE(snapshot->isSnapshotOfSameObject(*snapshotOfSameObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(*snapshotOfSameTypeObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(snapshotOfDifferentTypeObject));
}

}
