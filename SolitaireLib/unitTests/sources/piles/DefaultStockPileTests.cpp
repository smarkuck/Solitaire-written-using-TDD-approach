#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/DefaultStockPile.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire::piles {

class DefaultStockPileTest: public Test {
public:
    DefaultStockPile pile;
};

TEST_F(DefaultStockPileTest, createPile) {
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileTest, selectNextCard) {
    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

class DefaultStockPileWithCardsTest: public DefaultStockPileTest {
public:
    DefaultStockPileWithCardsTest() {
        pile.initialize(pileCards.begin(), pileCards.end());
    }

    Cards pileCards {
        Card {Value::Ten, Suit::Heart},
        Card {Value::Three, Suit::Spade},
        Card {Value::Two, Suit::Diamond}
    };
};

TEST_F(DefaultStockPileWithCardsTest, initializePileWithCards) {
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, selectNextCard) {
    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 1);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 2);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutCardWhenNoneIsSelected) {
    EXPECT_EQ(pile.tryPullOutCard(), std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutFirstCard) {
    const auto firstCard = pileCards.front();
    pileCards.erase(pileCards.begin());

    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), firstCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutSecondCard) {
    const auto secondCardIt = std::next(pileCards.begin());
    const auto secondCard = *secondCardIt;
    pileCards.erase(secondCardIt);

    pile.selectNextCard();
    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), secondCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(DefaultStockPileWithCardsTest, tryPullOutLastCard) {
    const Card lastCard = pileCards.back();
    pileCards.pop_back();

    pile.selectNextCard();
    pile.selectNextCard();
    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), lastCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 1);
}

TEST_F(DefaultStockPileWithCardsTest, tryRestoreCardWhenNothingPulledOut)
{
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

class DefaultStockPileWithCardsAfterPullOutTest: public DefaultStockPileTest {
public:
    DefaultStockPileWithCardsAfterPullOutTest() {
        pile.initialize(pileCards.begin(), pileCards.end());
        pile.selectNextCard();
        pile.tryPullOutCard();

        pileCards.erase(pileCards.begin());
    }

    Cards pileCards {
        Card {Value::King, Suit::Club},
        Card {Value::Ten, Suit::Heart},
        Card {Value::Three, Suit::Spade},
        Card {Value::Two, Suit::Diamond}
    };
};

TEST_F(DefaultStockPileWithCardsAfterPullOutTest,
       tryRestoreFirstPulledOutCardTwoTimes)
{
    pile.selectNextCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);

    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(DefaultStockPileWithCardsAfterPullOutTest, tryRestoreLastPulledOutCard) {
    pile.selectNextCard();
    pile.selectNextCard();
    pile.selectNextCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 2);
}

TEST_F(DefaultStockPileWithCardsAfterPullOutTest, tryRestoreCardWhenPullOutFailed) {
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(DefaultStockPileWithCardsAfterPullOutTest,
       tryRestorePulledOutCardAfterSelectingNextCard)
{
    pile.selectNextCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(DefaultStockPileWithCardsAfterPullOutTest, initializePileAfterOperations) {
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };

    pile.initialize(newPileCards.begin(), newPileCards.end());
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

}
