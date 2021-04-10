#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/DefaultFoundationPile.h"

using namespace testing;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(DefaultFoundationPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    DefaultFoundationPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyDefaultFoundationPileTest: public Test {
public:
    std::shared_ptr<DefaultFoundationPile> pile {std::make_shared<DefaultFoundationPile>()};
};

TEST_F(EmptyDefaultFoundationPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getTopCardValue(), std::nullopt);
}

TEST_F(EmptyDefaultFoundationPileTest, tryAddNotCard) {
    std::optional<Card> notCard = std::nullopt;

    pile->tryAddCard(notCard);

    EXPECT_EQ(notCard, std::nullopt);
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyDefaultFoundationPileTest, tryAddNotAce) {
    const Card cardToAddAfterOperation {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = cardToAddAfterOperation;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardToAddAfterOperation);
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyDefaultFoundationPileTest, tryAddAce) {
    const Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };

    std::optional<Card> cardToAdd = pileCards.front();

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), Value::Ace);
}

TEST_F(EmptyDefaultFoundationPileTest, tryPullOutCard) {
    EXPECT_EQ(pile->tryPullOutCard(), std::nullopt);
    EXPECT_TRUE(pile->getCards().empty());
}

class DefaultFoundationPileWithAceTest: public EmptyDefaultFoundationPileTest {
public:
    DefaultFoundationPileWithAceTest() {
        std::optional<Card> cardToAdd = pileCards.front();
        pile->tryAddCard(cardToAdd);
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };
};

TEST_F(DefaultFoundationPileWithAceTest, tryAddSameCardAsOnTopOfPile) {
    const auto sameCardAsOnTopOfPile = pileCards.back();
    std::optional<Card> cardToAdd = sameCardAsOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, sameCardAsOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest,
       tryAddCardWithValueTwoGreaterThanOnTopOfPile)
{
    const Card cardWithValueTwoGreaterThanOnTopOfPile {Value::Three, Suit::Heart};
    std::optional<Card> cardToAdd = cardWithValueTwoGreaterThanOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithValueTwoGreaterThanOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest,
       tryAddCardWithDifferentSuitThanOnTopOfPile)
{
    const Card cardWithDifferentSuitThanOnTopOfPile {Value::Two, Suit::Spade};
    std::optional<Card> cardToAdd = cardWithDifferentSuitThanOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithDifferentSuitThanOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithAceTest, tryAddTwoWithSameSuitAsOnTopOfPile) {
    pileCards.push_back(Card {Value::Two, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), Value::Two);
}

TEST_F(DefaultFoundationPileWithAceTest, tryPullOutCard) {
    const auto pulledOutCard = pileCards.back();
    EXPECT_EQ(pile->tryPullOutCard(), pulledOutCard);
    EXPECT_TRUE(pile->getCards().empty());
}

class DefaultFoundationPileWithTwoTest: public DefaultFoundationPileWithAceTest
{
public:
    DefaultFoundationPileWithTwoTest() {
        pileCards.push_back(Card {Value::Two, Suit::Heart});

        std::optional<Card> cardToAdd = pileCards.back();
        pile->tryAddCard(cardToAdd);
    }
};

TEST_F(DefaultFoundationPileWithTwoTest, tryPullOutCard) {
    const Card pulledOutCard = pileCards.back();
    pileCards.pop_back();

    EXPECT_EQ(pile->tryPullOutCard(), pulledOutCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(DefaultFoundationPileWithTwoTest, initializePileAfterOperations) {
    pile->initialize();

    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getTopCardValue(), std::nullopt);
}

TEST_F(DefaultFoundationPileWithTwoTest, restorePileStateUsingSnapshot) {
    const auto snapshot = pile->createSnapshot();
    pile->initialize();
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), pileCards.back().getValue());
}

}
