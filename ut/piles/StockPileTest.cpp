#include "gmock/gmock.h"
#include "piles/StockPile.h"

using namespace testing;
using namespace solitaire::card;

namespace solitaire {
namespace piles {

class StockPileTest: public Test {
public:
    StockPile pile;
};

TEST_F(StockPileTest, createPile) {
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileTest, selectNextCard) {
    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

class StockPileWithCardsTest: public StockPileTest {
public:
    StockPileWithCardsTest() {
        pile.initialize(pileCards.begin(), pileCards.end());
    }

    Cards pileCards {
        Card {Value::Ten, Suit::Heart},
        Card {Value::Three, Suit::Spade},
        Card {Value::Two, Suit::Diamond}
    };
};

TEST_F(StockPileWithCardsTest, initializePileWithCards) {
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, selectNextCard) {
    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 1);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), 2);

    pile.selectNextCard();
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutCardWhenNoneIsSelected) {
    EXPECT_EQ(pile.tryPullOutCard(), std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutFirstCard) {
    const auto firstCard = pileCards.front();
    pileCards.erase(pileCards.begin());

    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), firstCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsTest, tryPullOutSecondCard) {
    const auto secondCardIt = std::next(pileCards.begin());
    const auto secondCard = *secondCardIt;
    pileCards.erase(secondCardIt);

    pile.selectNextCard();
    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), secondCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(StockPileWithCardsTest, tryPullOutLastCard) {
    const Card lastCard = pileCards.back();
    pileCards.pop_back();

    pile.selectNextCard();
    pile.selectNextCard();
    pile.selectNextCard();

    EXPECT_EQ(pile.tryPullOutCard(), lastCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 1);
}

TEST_F(StockPileWithCardsTest, tryRestoreCardWhenNothingPulledOut)
{
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

class StockPileWithCardsAfterPullOutTest: public StockPileTest {
public:
    StockPileWithCardsAfterPullOutTest() {
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

TEST_F(StockPileWithCardsAfterPullOutTest, tryRestoreFirstPulledOutCardTwoTimes) {
    pile.selectNextCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);

    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(StockPileWithCardsAfterPullOutTest, tryRestoreLastPulledOutCard) {
    pile.selectNextCard();
    pile.selectNextCard();
    pile.selectNextCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 2);
}

TEST_F(StockPileWithCardsAfterPullOutTest, tryRestoreCardWhenPullOutFailed) {
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), std::nullopt);
}

TEST_F(StockPileWithCardsAfterPullOutTest,
       tryRestorePulledOutCardAfterSelectingNextCard)
{
    pile.selectNextCard();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getSelectedCardIndex(), 0);
}

TEST_F(StockPileWithCardsAfterPullOutTest, initializePileAfterOperations) {
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
}
