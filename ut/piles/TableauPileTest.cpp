#include <tuple>

#include "gmock/gmock.h"
#include "piles/TableauPile.h"

using namespace testing;
using namespace solitaire::card;

namespace solitaire {
namespace piles {

namespace {
    const Cards noCards;
}

TEST(TableauPileTest, createEmptyPile) {
    TableauPile pile{noCards.begin(), noCards.begin()};
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST(TableauPileTest, createPileWithOneCard) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

TEST(TableauPileTest, createPileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

TEST(TableauPileTest, createPileWithSevenCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade},
        Card {Value::Three, Suit::Diamond},
        Card {Value::Four, Suit::Club},
        Card {Value::Five, Suit::Heart},
        Card {Value::Six, Suit::Spade},
        Card {Value::Seven, Suit::Diamond}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 6);
}

class EmptyTableauPileTest: public Test {
public:
    TableauPile pile {noCards.begin(), noCards.end()};
};

TEST_F(EmptyTableauPileTest, tryAddNoCards) {
    Cards noCardsToAdd;
    pile.tryAddCards(noCardsToAdd);

    EXPECT_TRUE(noCardsToAdd.empty());
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(EmptyTableauPileTest, tryAddCardsWhenKingIsNotFirst) {
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

TEST_F(EmptyTableauPileTest, tryAddCardsWhenKingIsFirst) {
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

TEST_F(EmptyTableauPileTest, tryUncoverTopCard) {
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 0);
}

class TableauPileTestBase: public Test {
public:
    Cards pileCards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Ten, Suit::Spade},
        Card {Value::Five, Suit::Diamond}
    };
};

class TableauPileWithUncoveredTopCardTest: public TableauPileTestBase {
public:
    Cards concatenateCards(const Cards& lhs, const Cards& rhs) {
        auto result = lhs;
        result.insert(result.end(), rhs.begin(), rhs.end());
        return result;
    }

    Cards cardsToAdd {
        Card {Value::Four, Suit::Club},
        Card {Value::Three, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };
};

TEST_F(TableauPileWithUncoveredTopCardTest, tryAddCardsWhenFirstCardColorIsIncorrect) {
    pileCards.back() = Card {pileCards.back().getValue(), Suit::Heart};
    TableauPile pile {pileCards.begin(), pileCards.end()};

    cardsToAdd.front() = Card {cardsToAdd.front().getValue(), Suit::Diamond};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopCardTest, tryAddCardsWhenFirstCardValueIsIncorrect) {
    pileCards.back() = Card {Value::Five, pileCards.back().getSuit()};
    TableauPile pile {pileCards.begin(), pileCards.end()};

    cardsToAdd.front() = Card {Value::Three, cardsToAdd.front().getSuit()};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopCardTest, tryAddCardsWhenFirstCardIsCorrect) {
    TableauPile pile{pileCards.begin(), pileCards.end()};
    const auto cardsInPileAfterAdding = concatenateCards(pileCards, cardsToAdd);

    pile.tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(cardsInPileAfterAdding));
}

class TableauPileTestBaseWithCreatedPile: public TableauPileTestBase {
public:
    TableauPile pile {pileCards.begin(), pileCards.end()};
};

class TableauPileWithUncoveredTopTwoCardsTest: public TableauPileTestBaseWithCreatedPile {
public:
    TableauPileWithUncoveredTopTwoCardsTest() {
        Cards cardsToAdd {
            Card {Value::Four, Suit::Club}
        };

        pileCards.push_back(cardsToAdd.back());
        pile.tryAddCards(cardsToAdd);
    }
};

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutZeroCards) {
    EXPECT_TRUE(pile.tryPullOutCards(0).empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutOneCard) {
    const Cards pulledOutCards {std::prev(pileCards.end()), pileCards.end()};
    pileCards.pop_back();

    EXPECT_THAT(pile.tryPullOutCards(1), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutTwoCards) {
    const auto quantityOfCardsToPullOut = 2u;

    const auto firstCardToPullOut = std::prev(pileCards.end(), quantityOfCardsToPullOut);
    const Cards pulledOutCards {firstCardToPullOut, pileCards.end()};
    pileCards.erase(firstCardToPullOut, pileCards.end());

    EXPECT_THAT(pile.tryPullOutCards(quantityOfCardsToPullOut), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutTooMuchCards) {
    EXPECT_TRUE(pile.tryPullOutCards(3).empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

class TableauPileWithCoveredTopCardTest: public TableauPileTestBaseWithCreatedPile {
public:
    TableauPileWithCoveredTopCardTest() {
        pileCards.pop_back();
        pile.tryPullOutCards(1);
    }
};

TEST_F(TableauPileWithCoveredTopCardTest, tryAddCardsOnCoveredTopCard) {
    Cards cardsToAdd {
        Card {Value::Nine, Suit::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithCoveredTopCardTest, tryUncoverTopCard) {
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 2);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);

    pile.tryUncoverTopCard();
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

class TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest:
    public TableauPileWithUncoveredTopTwoCardsTest
{
public:
    TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest() {
        Cards cardsToAdd {
            Card {Value::Three, Suit::Heart}
        };

        pile.tryAddCards(cardsToAdd);
        pile.tryPullOutCards(1);
    }
};

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreZeroPulledOutCards)
{
    pile.tryPullOutCards(0);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreTwoPulledOutCardsTwoTimes)
{
    pile.tryPullOutCards(2);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));

    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestoreCardsWhenPullOutFailed)
{
    pile.tryPullOutCards(3);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestorePulledOutCardsAfterUncoverOperation)
{
    pile.tryUncoverTopCard();
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest,
       tryRestorePulledOutCardsAfterAddOperation)
{
    Cards noCards;
    pile.tryAddCards(noCards);
    pile.tryRestoreLastPulledOutCards();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsAndOnePulledOutTest, resetPile) {
    const Cards newPileCards {
        Card {Value::Six, Suit::Club},
        Card {Value::Ten, Suit::Spade}
    };

    pile.reset(newPileCards.begin(), newPileCards.end());
    pile.tryRestoreLastPulledOutCards();

    EXPECT_THAT(pile.getCards(), ContainerEq(newPileCards));
    EXPECT_EQ(pile.getPlaceInOrderOfFirstCoveredCard(), 1);
}

}
}
