#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/DefaultTableauPile.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire::piles {

class DefaultTableauPileTest: public Test {
public:
    DefaultTableauPile pile;
};

TEST_F(DefaultTableauPileTest, createPile) {
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST_F(DefaultTableauPileTest, initializePileWithOneCard) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart}
    };

    pile.initialize(cards.begin(), cards.end());
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST_F(DefaultTableauPileTest, initializePileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };

    pile.initialize(cards.begin(), cards.end());
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

TEST_F(DefaultTableauPileTest, initializePileWithSevenCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade},
        Card {Value::Three, Suit::Diamond},
        Card {Value::Four, Suit::Club},
        Card {Value::Five, Suit::Heart},
        Card {Value::Six, Suit::Spade},
        Card {Value::Seven, Suit::Diamond}
    };

    pile.initialize(cards.begin(), cards.end());
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 6);
}

TEST_F(DefaultTableauPileTest, tryAddNoCards) {
    Cards noCardsToAdd;
    pile.tryAddCards(noCardsToAdd);

    EXPECT_TRUE(noCardsToAdd.empty());
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(DefaultTableauPileTest, tryAddCardsWhenKingIsNotFirst) {
    Cards cardsToAdd {
        Card {Value::Queen, Suit::Heart},
        Card {Value::King, Suit::Spade},
        Card {Value::Jack, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(DefaultTableauPileTest, tryAddCardsWhenKingIsFirst) {
    Cards cardsToAdd {
        Card {Value::King, Suit::Heart},
        Card {Value::Queen, Suit::Spade},
        Card {Value::Jack, Suit::Diamond}
    };
    const auto cardsInPileAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(cardsInPileAfterAdding));
}

TEST_F(DefaultTableauPileTest, tryUncoverTopCard) {
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

class InitializedDefaultTableauPileTest: public DefaultTableauPileTest {
public:
    InitializedDefaultTableauPileTest() {
        pile.initialize(pileCards.begin(), pileCards.end());
    }

    Cards concatenateCards(const Cards& lhs, const Cards& rhs) {
        auto result = lhs;
        result.insert(result.end(), rhs.begin(), rhs.end());
        return result;
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Ten, Suit::Spade},
        Card {Value::Five, Suit::Diamond}
    };
};

class DefaultTableauPileWithUncoveredTopCardTest:
    public InitializedDefaultTableauPileTest
{
public:
    Cards cardsToAdd {
        Card {Value::Four, Suit::Club},
        Card {Value::Three, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };
};

TEST_F(DefaultTableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardColorIsIncorrect)
{
    cardsToAdd.front() = Card {cardsToAdd.front().getValue(), Suit::Diamond};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardValueIsIncorrect)
{
    cardsToAdd.front() = Card {Value::Three, cardsToAdd.front().getSuit()};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardIsCorrect)
{
    const auto cardsInPileAfterAdding = concatenateCards(pileCards, cardsToAdd);

    pile.tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(cardsInPileAfterAdding));
}

class DefaultTableauPileWithCoveredTopCardTest:
    public InitializedDefaultTableauPileTest
{
public:
    DefaultTableauPileWithCoveredTopCardTest() {
        pileCards.pop_back();
        pile.tryPullOutCards(1);
    }
};

TEST_F(DefaultTableauPileWithCoveredTopCardTest, tryAddCardsOnCoveredTopCard) {
    Cards cardsToAdd {
        Card {Value::Nine, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithCoveredTopCardTest, tryUncoverTopCard) {
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 2);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

class DefaultTableauPileWithUncoveredTopTwoCardsTest:
    public InitializedDefaultTableauPileTest
{
public:
    DefaultTableauPileWithUncoveredTopTwoCardsTest() {
        Cards cardsToAdd {
            Card {Value::Four, Suit::Club}
        };

        pileCards.push_back(cardsToAdd.back());
        pile.tryAddCards(cardsToAdd);
    }
};

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutZeroCards) {
    EXPECT_TRUE(pile.tryPullOutCards(0).empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutOneCard) {
    const Cards pulledOutCards {pileCards.back()};
    pileCards.pop_back();

    EXPECT_THAT(pile.tryPullOutCards(1), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutTwoCards) {
    const auto quantityOfCardsToPullOut = 2u;

    const auto firstCardToPullOut = std::prev(pileCards.end(), quantityOfCardsToPullOut);
    const Cards pulledOutCards {firstCardToPullOut, pileCards.end()};
    pileCards.erase(firstCardToPullOut, pileCards.end());

    EXPECT_THAT(pile.tryPullOutCards(quantityOfCardsToPullOut), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutTooMuchCards) {
    EXPECT_TRUE(pile.tryPullOutCards(3).empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest,
       tryRestoreCardWhenNothingPulledOut)
{
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

class DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest:
    public DefaultTableauPileWithUncoveredTopTwoCardsTest
{
public:
    DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest() {
        Cards cardsToAdd {
            Card {Value::Three, Suit::Heart}
        };

        pile.tryAddCards(cardsToAdd);
        pile.tryPullOutCards(1);
    }
};

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreZeroPulledOutCards)
{
    pile.tryPullOutCards(0);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreTwoPulledOutCardsTwoTimes)
{
    pile.tryPullOutCards(2);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));

    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreCardsWhenPullOutFailed)
{
    pile.tryPullOutCards(3);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestorePulledOutCardsAfterUncoverOperation)
{
    pile.tryUncoverTopCard();
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestorePulledOutCardsAfterAddOperation)
{
    pileCards.push_back(Card {Value::Three, Suit::Heart});
    Cards cardsToAdd {pileCards.back()};

    pile.tryAddCards(cardsToAdd);
    pile.tryRestoreLastPulledOutCards();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       initializePileAfterOperations)
{
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };

    pile.initialize(newPileCards.begin(), newPileCards.end());
    pile.tryRestoreLastPulledOutCards();

    EXPECT_THAT(pile.getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

}
