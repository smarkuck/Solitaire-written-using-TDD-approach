#include "gmock/gmock.h"
#include "piles/DefaultFoundationPile.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire {
namespace piles {

class DefaultFoundationPileTest: public Test {
public:
    DefaultFoundationPile pile;
};

TEST_F(DefaultFoundationPileTest, createPile) {
    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getTopCardValue(), std::nullopt);
}

TEST_F(DefaultFoundationPileTest, tryAddNotCard) {
    std::optional<Card> notCard = std::nullopt;

    pile.tryAddCard(notCard);

    EXPECT_EQ(notCard, std::nullopt);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(DefaultFoundationPileTest, tryAddNotAce) {
    const Card cardToAddAfterOperation {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = cardToAddAfterOperation;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardToAddAfterOperation);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(DefaultFoundationPileTest, tryAddAce) {
    const Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };

    std::optional<Card> cardToAdd = pileCards.front();

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getTopCardValue(), Value::Ace);
}

TEST_F(DefaultFoundationPileTest, tryPullOutCard) {
    EXPECT_EQ(pile.tryPullOutCard(), std::nullopt);
    EXPECT_TRUE(pile.getCards().empty());
}

class DefaultFoundationPileWithAceTest: public DefaultFoundationPileTest {
public:
    DefaultFoundationPileWithAceTest() {
        std::optional<Card> cardToAdd = pileCards.front();
        pile.tryAddCard(cardToAdd);
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };
};

TEST_F(DefaultFoundationPileWithAceTest, tryAddSameCardAsOnTopOfPile) {
    const auto sameCardAsOnTopOfPile = pileCards.back();
    std::optional<Card> cardToAdd = sameCardAsOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, sameCardAsOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest,
       tryAddCardWithValueTwoGreaterThanOnTopOfPile)
{
    const Card cardWithValueTwoGreaterThanOnTopOfPile {Value::Three, Suit::Heart};
    std::optional<Card> cardToAdd = cardWithValueTwoGreaterThanOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithValueTwoGreaterThanOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest,
       tryAddCardWithDifferentSuitThanOnTopOfPile)
{
    const Card cardWithDifferentSuitThanOnTopOfPile {Value::Two, Suit::Spade};
    std::optional<Card> cardToAdd = cardWithDifferentSuitThanOnTopOfPile;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithDifferentSuitThanOnTopOfPile);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest, tryAddTwoWithSameSuitAsOnTopOfPile) {
    pileCards.push_back(Card {Value::Two, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile.getTopCardValue(), Value::Two);
}

TEST_F(DefaultFoundationPileWithAceTest, tryPullOutCard) {
    const auto pulledOutCard = pileCards.back();
    EXPECT_EQ(pile.tryPullOutCard(), pulledOutCard);
    EXPECT_TRUE(pile.getCards().empty());
}

TEST_F(DefaultFoundationPileWithAceTest, tryPullOutCardFromPileWithTwoOnTop) {
    const Card pulledOutCard = Card {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = pulledOutCard;

    pile.tryAddCard(cardToAdd);

    EXPECT_EQ(pile.tryPullOutCard(), pulledOutCard);
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest, tryRestoreCardWhenNothingPulledOut) {
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest, tryRestorePulledOutAce) {
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest, tryRestoreCardWhenPullOutFailed) {
    pile.tryPullOutCard();
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_TRUE(pile.getCards().empty());
}

class DefaultFoundationPileWithTwoAndPulledOutThreeTest:
    public DefaultFoundationPileWithAceTest
{
public:
    DefaultFoundationPileWithTwoAndPulledOutThreeTest() {
        pileCards.push_back(Card {Value::Two, Suit::Heart});

        std::optional<Card> cardToAdd = pileCards.back();
        pile.tryAddCard(cardToAdd);

        cardToAdd = Card {Value::Three, Suit::Heart};
        pile.tryAddCard(cardToAdd);
        pile.tryPullOutCard();
    }
};

TEST_F(DefaultFoundationPileWithTwoAndPulledOutThreeTest,
       tryRestoreLastPulledOutCardTwoTimes)
{
    pile.tryPullOutCard();
    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));

    pile.tryRestoreLastPulledOutCard();
    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithTwoAndPulledOutThreeTest,
       tryRestorePulledOutCardAfterAddOperation)
{
    pileCards.push_back(Card {Value::Three, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile.tryAddCard(cardToAdd);
    pile.tryRestoreLastPulledOutCard();

    EXPECT_THAT(pile.getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithTwoAndPulledOutThreeTest,
       initializePileAfterOperations)
{
    pile.initialize();
    pile.tryRestoreLastPulledOutCard();

    EXPECT_TRUE(pile.getCards().empty());
    EXPECT_EQ(pile.getTopCardValue(), std::nullopt);
}

}
}