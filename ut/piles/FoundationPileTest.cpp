#include "gmock/gmock.h"
#include "piles/FoundationPile.h"

using namespace testing;
using namespace solitaire::card;

namespace solitaire {
namespace piles {

class EmptyFoundationPileTest: public Test {
public:
    FoundationPile pile;
};

TEST_F(EmptyFoundationPileTest, createPile) {
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getTopCardValue(), std::nullopt);
}

TEST_F(EmptyFoundationPileTest, tryAddNotCard) {
    std::optional<Card> notCard = std::nullopt;

    pile.tryAddCard(notCard);

    EXPECT_EQ(notCard, std::nullopt);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(EmptyFoundationPileTest, tryAddNotAce) {
    const Card cardToAddAfterOperation {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = cardToAddAfterOperation;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardToAddAfterOperation);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(EmptyFoundationPileTest, tryAddAce) {
    const Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };

    std::optional<Card> cardToAdd = pileCards.front();

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getTopCardValue(), Value::Ace);
}

TEST_F(EmptyFoundationPileTest, tryPullOutCard) {
    EXPECT_EQ(pile.tryPullOutCard(), std::nullopt);
    EXPECT_TRUE(pile.getCards().empty());
}

class FoundationPileWithAceTest: public EmptyFoundationPileTest {
public:
    FoundationPileWithAceTest() {
        std::optional<Card> cardToAdd = pileCards.front();
        pile.tryAddCard(cardToAdd);
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };
};

TEST_F(FoundationPileWithAceTest, tryAddSameCardAsOnTopOfPile) {
    const auto sameCardAsOnTopOfPile = pileCards.back();
    std::optional<Card> cardToAdd = sameCardAsOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, sameCardAsOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryAddCardWithValueTwoGreaterThanOnTopOfPile) {
    const Card cardWithValueTwoGreaterThanOnTopOfPile {Value::Three, Suit::Heart};
    std::optional<Card> cardToAdd = cardWithValueTwoGreaterThanOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithValueTwoGreaterThanOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryAddCardWithDifferentSuitThanOnTopOfPile) {
    const Card cardWithDifferentSuitThanOnTopOfPile {Value::Two, Suit::Spade};
    std::optional<Card> cardToAdd = cardWithDifferentSuitThanOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithDifferentSuitThanOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryAddTwoWithSameSuitAsOnTopOfPile) {
    pileCards.push_back(Card {Value::Two, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getTopCardValue(), Value::Two);
}

TEST_F(FoundationPileWithAceTest, tryPullOutCard) {
    const auto pulledOutCard = pileCards.back();
    EXPECT_EQ(pile.tryPullOutCard(), pulledOutCard);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(FoundationPileWithAceTest, tryPullOutCardFromPileWithTwoOnTop) {
    const Card pulledOutCard = Card {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = pulledOutCard;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(pile.tryPullOutCard(), pulledOutCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryRestoreCardWhenNothingPulledOut) {
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryRestorePulledOutAce) {
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryRestoreCardWhenPullOutFailed) {
    pile.tryPullOutCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_TRUE(pile.getCards().empty());
}

class FoundationPileWithTwoAndPulledOutThreeTest: public FoundationPileWithAceTest {
public:
    FoundationPileWithTwoAndPulledOutThreeTest() {
        pileCards.push_back(Card {Value::Two, Suit::Heart});

        std::optional<Card> cardToAdd = pileCards.back();
        pile.tryAddCard(cardToAdd);

        cardToAdd = Card {Value::Three, Suit::Heart};
        pile.tryAddCard(cardToAdd);
        pile.tryPullOutCard();
    }
};

TEST_F(FoundationPileWithTwoAndPulledOutThreeTest,
       tryRestoreLastPulledOutCardTwoTimes)
{
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));

    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithTwoAndPulledOutThreeTest,
       tryRestorePulledOutCardAfterAddOperation)
{
    pileCards.push_back(Card {Value::Three, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile.tryAddCard(cardToAdd);
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithTwoAndPulledOutThreeTest, resetPile) {
    pile.reset();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getTopCardValue(), std::nullopt);
}

}
}
