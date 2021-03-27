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
        Card {Value::Ace, Color::Heart}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

TEST(TableauPileTest, createPileWithTwoCards) {
    const Cards cards {
        Card {Value::Ace, Color::Heart},
        Card {Value::Two, Color::Spade}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 1);
}

TEST(TableauPileTest, createPileWithSevenCards) {
    const Cards cards {
        Card {Value::Ace, Color::Heart},
        Card {Value::Two, Color::Spade},
        Card {Value::Three, Color::Diamond},
        Card {Value::Four, Color::Club},
        Card {Value::Five, Color::Heart},
        Card {Value::Six, Color::Spade},
        Card {Value::Seven, Color::Diamond}
    };

    TableauPile pile{cards.begin(), cards.end()};
    EXPECT_THAT(pile.getCards(), ContainerEq(cards));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 6);
}

TEST(TableauPileTest, tryAddNoCards) {
    TableauPile pile{noCards.begin(), noCards.end()};

    Cards cardsToAdd;
    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(noCards));
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

TEST(TableauPileTest, tryAddCardsWhenKingIsNotFirstToEmptyPile) {
    TableauPile pile{noCards.begin(), noCards.end()};

    Cards cardsToAdd {
        Card {Value::Three, Color::Heart},
        Card {Value::Two, Color::Spade},
        Card {Value::Ace, Color::Diamond}
    };
    const auto cardsLeftAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(cardsLeftAfterAdding));
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

TEST(TableauPileTest, tryAddCardsWhenKingIsFirstToEmptyPile) {
    TableauPile pile{noCards.begin(), noCards.end()};

    Cards cardsToAdd {
        Card {Value::King, Color::Heart},
        Card {Value::Queen, Color::Spade},
        Card {Value::Jack, Color::Diamond}
    };
    const auto cardsInPileAfterAdding = cardsToAdd;

    pile.tryAddCards(cardsToAdd);

    EXPECT_THAT(cardsToAdd, ContainerEq(noCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(cardsInPileAfterAdding));
    EXPECT_EQ(pile.getQueuePositionOfFirstFaceDownCard(), 0);
}

}
}
