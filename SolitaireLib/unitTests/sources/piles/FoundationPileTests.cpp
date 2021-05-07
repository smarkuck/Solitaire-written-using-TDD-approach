#include "archivers/SnapshotMock.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/FoundationPile.h"

using namespace testing;
using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

TEST(FoundationPileTest,
     throwExceptionOnSnapshotCreationWhenPileNotCreatedBySharedPtr)
{
    FoundationPile pile;
    EXPECT_THROW(pile.createSnapshot(), std::bad_weak_ptr);
}

class EmptyFoundationPileTest: public Test {
public:
    std::shared_ptr<FoundationPile> pile {std::make_shared<FoundationPile>()};
};

TEST_F(EmptyFoundationPileTest, createPile) {
    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getTopCardValue(), std::nullopt);
}

TEST_F(EmptyFoundationPileTest, tryAddNotCard) {
    std::optional<Card> notCard = std::nullopt;

    pile->tryAddCard(notCard);

    EXPECT_EQ(notCard, std::nullopt);
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyFoundationPileTest, tryAddNotAce) {
    const Card cardToAddAfterOperation {Value::Two, Suit::Heart};
    std::optional<Card> cardToAdd = cardToAddAfterOperation;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardToAddAfterOperation);
    EXPECT_TRUE(pile->getCards().empty());
}

TEST_F(EmptyFoundationPileTest, tryAddAce) {
    const Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };

    std::optional<Card> cardToAdd = pileCards.front();

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), Value::Ace);
}

TEST_F(EmptyFoundationPileTest, tryPullOutCard) {
    EXPECT_EQ(pile->tryPullOutCard(), std::nullopt);
    EXPECT_TRUE(pile->getCards().empty());
}

class FoundationPileWithAceTest: public EmptyFoundationPileTest {
public:
    FoundationPileWithAceTest() {
        std::optional<Card> cardToAdd = pileCards.front();
        pile->tryAddCard(cardToAdd);
    }

    Cards pileCards {
        Card {Value::Ace, Suit::Heart}
    };
};

TEST_F(FoundationPileWithAceTest, tryAddSameCardAsOnTopOfPile) {
    const auto sameCardAsOnTopOfPile = pileCards.back();
    std::optional<Card> cardToAdd = sameCardAsOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, sameCardAsOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest,
       tryAddCardWithValueTwoGreaterThanOnTopOfPile)
{
    const Card cardWithValueTwoGreaterThanOnTopOfPile {Value::Three, Suit::Heart};
    std::optional<Card> cardToAdd = cardWithValueTwoGreaterThanOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithValueTwoGreaterThanOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest,
       tryAddCardWithDifferentSuitThanOnTopOfPile)
{
    const Card cardWithDifferentSuitThanOnTopOfPile {Value::Two, Suit::Spade};
    std::optional<Card> cardToAdd = cardWithDifferentSuitThanOnTopOfPile;

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, cardWithDifferentSuitThanOnTopOfPile);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithAceTest, tryAddTwoWithSameSuitAsOnTopOfPile) {
    pileCards.push_back(Card {Value::Two, Suit::Heart});
    std::optional<Card> cardToAdd = pileCards.back();

    pile->tryAddCard(cardToAdd);

    EXPECT_EQ(cardToAdd, std::nullopt);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), Value::Two);
}

TEST_F(FoundationPileWithAceTest, tryPullOutCard) {
    const auto pulledOutCard = pileCards.back();
    EXPECT_EQ(pile->tryPullOutCard(), pulledOutCard);
    EXPECT_TRUE(pile->getCards().empty());
}

class FoundationPileWithTwoTest: public FoundationPileWithAceTest
{
public:
    FoundationPileWithTwoTest() {
        pileCards.push_back(Card {Value::Two, Suit::Heart});

        std::optional<Card> cardToAdd = pileCards.back();
        pile->tryAddCard(cardToAdd);
    }
};

TEST_F(FoundationPileWithTwoTest, tryPullOutCard) {
    const Card pulledOutCard = pileCards.back();
    pileCards.pop_back();

    EXPECT_EQ(pile->tryPullOutCard(), pulledOutCard);
    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
}

TEST_F(FoundationPileWithTwoTest, initializePileAfterOperations) {
    pile->initialize();

    EXPECT_TRUE(pile->getCards().empty());
    EXPECT_EQ(pile->getTopCardValue(), std::nullopt);
}

TEST_F(FoundationPileWithTwoTest, restorePileStateUsingSnapshot) {
    const auto snapshot = pile->createSnapshot();
    pile->initialize();
    snapshot->restore();

    EXPECT_THAT(pile->getCards(), ContainerEq(pileCards));
    EXPECT_EQ(pile->getTopCardValue(), pileCards.back().getValue());
}

TEST_F(FoundationPileWithTwoTest, isSnapshotOfSameObject) {
    const auto snapshot = pile->createSnapshot();
    pile->initialize();
    const auto snapshotOfSameObject = pile->createSnapshot();
    const auto snapshotOfSameTypeObject = std::make_shared<FoundationPile>()->createSnapshot();
    SnapshotMock snapshotOfDifferentTypeObject;

    EXPECT_TRUE(snapshot->isSnapshotOfSameObject(*snapshotOfSameObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(*snapshotOfSameTypeObject));
    EXPECT_FALSE(snapshot->isSnapshotOfSameObject(snapshotOfDifferentTypeObject));
}

}
