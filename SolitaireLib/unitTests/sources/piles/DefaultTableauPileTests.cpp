#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/DefaultTableauPile.h"
#include "piles/PileUtils.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(DefaultTableauPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    DefaultTableauPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyDefaultTableauPileTest: public Test {
public:
    std::shared_ptr<DefaultTableauPile> pile {std::make_shared<DefaultTableauPile>()};
};

TEST_F(EmptyDefaultTableauPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST_F(EmptyDefaultTableauPileTest, initializePileWithOneCard) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart}
    };

    initializePile(*pile, cards);
    EXPECT_THAT(pile->getCards(), ContainerEq(cards));
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST_F(EmptyDefaultTableauPileTest, initializePileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };

    initializePile(*pile, cards);
    EXPECT_THAT(pile->getCards(), ContainerEq(cards));
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 1);
}

TEST_F(EmptyDefaultTableauPileTest, initializePileWithSevenCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade},
        Card {Value::Three, Suit::Diamond},
        Card {Value::Four, Suit::Club},
        Card {Value::Five, Suit::Heart},
        Card {Value::Six, Suit::Spade},
        Card {Value::Seven, Suit::Diamond}
    };

    initializePile(*pile, cards);
    EXPECT_THAT(pile->getCards(), ContainerEq(cards));
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 6);
}

TEST_F(EmptyDefaultTableauPileTest, tryAddNoCards) {
    Cards noCardsToAdd;
    pile->tryAddCards(noCardsToAdd);

    EXPECT_TRUE(noCardsToAdd.empty());
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyDefaultTableauPileTest, tryAddCardsWhenKingIsNotFirst) {
    Cards cardsToAdd {
        Card {Value::Queen, Suit::Heart},
        Card {Value::King, Suit::Spade},
        Card {Value::Jack, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyDefaultTableauPileTest, tryAddCardsWhenKingIsFirst) {
    Cards cardsToAdd {
        Card {Value::King, Suit::Heart},
        Card {Value::Queen, Suit::Spade},
        Card {Value::Jack, Suit::Diamond}
    };
    const auto cardsInPileAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(cardsInPileAfterAdding));
}

TEST_F(EmptyDefaultTableauPileTest, tryUncoverTopCard) {
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 0);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 0);
}

class InitializedDefaultTableauPileTest: public EmptyDefaultTableauPileTest {
public:
    InitializedDefaultTableauPileTest() {
        initializePile(*pile, pileCards);
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

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardValueIsIncorrect)
{
    cardsToAdd.front() = Card {Value::Three, cardsToAdd.front().getSuit()};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardIsCorrect)
{
    const auto cardsInPileAfterAdding = concatenateCards(pileCards, cardsToAdd);

    pile->tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(cardsInPileAfterAdding));
}

class DefaultTableauPileWithCoveredTopCardTest:
    public InitializedDefaultTableauPileTest
{
public:
    DefaultTableauPileWithCoveredTopCardTest() {
        pileCards.pop_back();
        pile->tryPullOutCards(1);
    }
};

TEST_F(DefaultTableauPileWithCoveredTopCardTest, tryAddCardsOnCoveredTopCard) {
    Cards cardsToAdd {
        Card {Value::Nine, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithCoveredTopCardTest, tryUncoverTopCard) {
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 2);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 1);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 1);
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
        pile->tryAddCards(cardsToAdd);
    }
};

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutZeroCards) {
    EXPECT_TRUE(pile->tryPullOutCards(0).empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutOneCard) {
    const Cards pulledOutCards {pileCards.back()};
    pileCards.pop_back();

    EXPECT_THAT(pile->tryPullOutCards(1), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutTwoCards) {
    const auto quantityOfCardsToPullOut = 2u;

    const auto firstCardToPullOut = std::prev(pileCards.end(), quantityOfCardsToPullOut);
    const Cards pulledOutCards {firstCardToPullOut, pileCards.end()};
    pileCards.erase(firstCardToPullOut, pileCards.end());

    EXPECT_THAT(pile->tryPullOutCards(quantityOfCardsToPullOut), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultTableauPileWithUncoveredTopTwoCardsTest, tryPullOutTooMuchCards) {
    EXPECT_TRUE(pile->tryPullOutCards(3).empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

class DefaultTableauPileInitializationTest:
    public DefaultTableauPileWithUncoveredTopTwoCardsTest
{
public:
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };
};

TEST_F(DefaultTableauPileInitializationTest, initializePileAfterOperations) {
    initializePile(*pile, newPileCards);
    EXPECT_THAT(pile->getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 1);
}

TEST_F(DefaultTableauPileInitializationTest, restorePileStateUsingSnapshot) {
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getPlaceInOrderOfFirstCoveredCard(), 2);
}

}
