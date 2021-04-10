#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/DefaultStockPile.h"
#include "piles/PileUtils.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(DefaultStockPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    DefaultStockPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyDefaultStockPileTest: public Test {
public:
    std::shared_ptr<DefaultStockPile> pile {std::make_shared<DefaultStockPile>()};
};

TEST_F(EmptyDefaultStockPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(EmptyDefaultStockPileTest, selectNextCard) {
    pile->selectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

class DefaultStockPileWithCardsTest: public EmptyDefaultStockPileTest {
public:
    DefaultStockPileWithCardsTest() {
        initializePile(*pile, pileCards);
    }

    Cards pileCards {
        Card {Value::Ten, Suit::Heart},
        Card {Value::Three, Suit::Spade},
        Card {Value::Two, Suit::Diamond}
    };
};

TEST_F(DefaultStockPileWithCardsTest, initializePileWithCards) {
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, selectNextCard) {
    pile->selectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);

    pile->selectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 1);

    pile->selectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), 2);

    pile->selectNextCard();
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutCardWhenNoneIsSelected) {
    EXPECT_EQ(pile->tryPullOutCard(), std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutFirstCard) {
    const auto firstCard = pileCards.front();
    pileCards.erase(pileCards.begin());

    pile->selectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), firstCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutSecondCard) {
    const auto secondCardIt = std::next(pileCards.begin());
    const auto secondCard = *secondCardIt;
    pileCards.erase(secondCardIt);

    pile->selectNextCard();
    pile->selectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), secondCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutLastCard) {
    const Card lastCard = pileCards.back();
    pileCards.pop_back();

    pile->selectNextCard();
    pile->selectNextCard();
    pile->selectNextCard();

    EXPECT_EQ(pile->tryPullOutCard(), lastCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 1);
}

class DefaultStockPileInitializationTest: public DefaultStockPileWithCardsTest {
public:
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };
};

TEST_F(DefaultStockPileInitializationTest, initializePileAfterOperations) {
    initializePile(*pile, newPileCards);

    EXPECT_THAT(pile->getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileInitializationTest, restorePileStateUsingSnapshot) {
    pile->selectNextCard();
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getSelectedCardIndex(), 0);
}

}
