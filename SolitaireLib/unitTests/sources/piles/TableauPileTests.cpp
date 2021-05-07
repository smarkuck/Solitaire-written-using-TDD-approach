#include "archivers/SnapshotMock.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/PileUtils.h"
#include "piles/TableauPile.h"

using namespace testing;
using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(TableauPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    TableauPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyTableauPileTest: public Test {
public:
    std::shared_ptr<TableauPile> pile {std::make_shared<TableauPile>()};
};

TEST_F(EmptyTableauPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 0);
}

TEST_F(EmptyTableauPileTest, initializePileWithOneCard) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart}
    };

    initializePile(*pile, cards);
    EXPECT_THAT(pile->getCards(), ContainerEq(cards));
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 0);
}

TEST_F(EmptyTableauPileTest, initializePileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };

    initializePile(*pile, cards);
    EXPECT_THAT(pile->getCards(), ContainerEq(cards));
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 1);
}

TEST_F(EmptyTableauPileTest, initializePileWithSevenCards) {
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
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 6);
}

TEST_F(EmptyTableauPileTest, tryAddNoCards) {
    Cards noCardsToAdd;
    pile->tryAddCards(noCardsToAdd);

    EXPECT_TRUE(noCardsToAdd.empty());
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyTableauPileTest, tryAddCardsWhenKingIsNotFirst) {
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

TEST_F(EmptyTableauPileTest, tryAddCardsWhenKingIsFirst) {
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

TEST_F(EmptyTableauPileTest, tryUncoverTopCard) {
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 0);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 0);
}

class InitializedTableauPileTest: public EmptyTableauPileTest {
public:
    InitializedTableauPileTest() {
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

class TableauPileWithUncoveredTopCardTest:
    public InitializedTableauPileTest
{
public:
    Cards cardsToAdd {
        Card {Value::Four, Suit::Club},
        Card {Value::Three, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };
};

TEST_F(TableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardColorIsIncorrect)
{
    cardsToAdd.front() = Card {cardsToAdd.front().getValue(), Suit::Diamond};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardValueIsIncorrect)
{
    cardsToAdd.front() = Card {Value::Three, cardsToAdd.front().getSuit()};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopCardTest,
       tryAddCardsWhenFirstCardIsCorrect)
{
    const auto cardsInPileAfterAdding = concatenateCards(pileCards, cardsToAdd);

    pile->tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(cardsInPileAfterAdding));
}

class TableauPileWithCoveredTopCardTest:
    public InitializedTableauPileTest
{
public:
    TableauPileWithCoveredTopCardTest() {
        pileCards.pop_back();
        pile->tryPullOutCards(1);
    }
};

TEST_F(TableauPileWithCoveredTopCardTest, tryAddCardsOnCoveredTopCard) {
    Cards cardsToAdd {
        Card {Value::Nine, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile->tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithCoveredTopCardTest, tryUncoverTopCard) {
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 2);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 1);

    pile->tryUncoverTopCard();
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 1);
}

class TableauPileWithUncoveredTopTwoCardsTest:
    public InitializedTableauPileTest
{
public:
    TableauPileWithUncoveredTopTwoCardsTest() {
        Cards cardsToAdd {
            Card {Value::Four, Suit::Club}
        };

        pileCards.push_back(cardsToAdd.back());
        pile->tryAddCards(cardsToAdd);
    }
};

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutZeroCards) {
    EXPECT_TRUE(pile->tryPullOutCards(0).empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutOneCard) {
    const Cards pulledOutCards {pileCards.back()};
    pileCards.pop_back();

    EXPECT_THAT(pile->tryPullOutCards(1), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutTwoCards) {
    const auto quantityOfCardsToPullOut = 2u;

    const auto firstCardToPullOut = std::prev(pileCards.end(), quantityOfCardsToPullOut);
    const Cards pulledOutCards {firstCardToPullOut, pileCards.end()};
    pileCards.erase(firstCardToPullOut, pileCards.end());

    EXPECT_THAT(pile->tryPullOutCards(quantityOfCardsToPullOut), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutTooMuchCards) {
    EXPECT_TRUE(pile->tryPullOutCards(3).empty());
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

class TableauPileInitializationTest:
    public TableauPileWithUncoveredTopTwoCardsTest
{
public:
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };
};

TEST_F(TableauPileInitializationTest, initializePileAfterOperations) {
    initializePile(*pile, newPileCards);
    EXPECT_THAT(pile->getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 1);
}

TEST_F(TableauPileInitializationTest, restorePileStateUsingSnapshot) {
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCoveredCardPosition(), 2);
}

TEST_F(TableauPileInitializationTest, isSnapshotOfSameObject) {
    const auto snapshot = pile->createSnapshot();
    initializePile(*pile, newPileCards);
    const auto snapshotOfSameObject = pile->createSnapshot();
    const auto snapshotOfSameTypeObject = std::make_shared<TableauPile>()->createSnapshot();
    SnapshotMock snapshotOfDifferentTypeObject;

    EXPECT_TRUE(snapshot->isSnapshotOfSameObject(*snapshotOfSameObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(*snapshotOfSameTypeObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(snapshotOfDifferentTypeObject));
}

}
