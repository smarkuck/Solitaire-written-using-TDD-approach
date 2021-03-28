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

class TableauPileWithUncoveredTopTwoCardsTest: public Test {
public:
    TableauPileWithUncoveredTopTwoCardsTest() {
        const Card cardToAdd {Value::Four, Suit::Club};
        Cards cardsToAdd {cardToAdd};

        pileCards.push_back(cardToAdd);
        pile.tryAddCards(cardsToAdd);
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart},
        Card {Value::Ten, Suit::Spade},
        Card {Value::Five, Suit::Diamond}
    };

    TableauPile pile {pileCards.begin(), pileCards.end()};
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

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutTwoCard) {
    const auto firstCardToPullOut = std::prev(pileCards.end(), 2);
    const Cards pulledOutCards {firstCardToPullOut, pileCards.end()};
    pileCards.erase(firstCardToPullOut, pileCards.end());

    EXPECT_THAT(pile.tryPullOutCards(2), ContainerEq(pulledOutCards));
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(TableauPileWithUncoveredTopTwoCardsTest, tryPullOutThreeCards) {
    EXPECT_TRUE(pile.tryPullOutCards(3).empty());
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

}
}
