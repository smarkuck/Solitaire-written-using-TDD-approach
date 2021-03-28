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
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

TEST(TableauPileTest, createPileWithOneCard) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

TEST(TableauPileTest, createPileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Two, Suit::Spade}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 1);
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
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 6);
}

class EmptyTableauPileTest: public Test {
public:
    EmptyTableauPileTest():
        pile {noCards.begin(), noCards.end()} {
    }

    TableauPile pile;
};

TEST_F(EmptyTableauPileTest, tryAddNoCards) {
    Cards noCardsToAdd;
    pile.tryAddCards(noCardsToAdd);

    EXPECT_TRUE(noCardsToAdd.empty());
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
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
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
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
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

class TableauPileWithUncoveredTopCardTest: public Test {
public:
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
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 2);
}

TEST_F(TableauPileWithUncoveredTopCardTest, tryAddCardsWhenFirstCardValueIsIncorrect) {
    pileCards.back() = Card {Value::Five, pileCards.back().getSuit()};
    TableauPile pile {pileCards.begin(), pileCards.end()};

    cardsToAdd.front() = Card {Value::Three, cardsToAdd.front().getSuit()};
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 2);
}

TEST_F(TableauPileWithUncoveredTopCardTest, tryAddCardsWhenFirstCardIsCorrect) {
    TableauPile pile{pileCards.begin(), pileCards.end()};
    const auto cardsInPileAfterAdding = concatenateCards(pileCards, cardsToAdd);

    pile.tryAddCards(cardsToAdd);

    EXPECT_TRUE(cardsToAdd.empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(cardsInPileAfterAdding));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 2);
}

}
}
