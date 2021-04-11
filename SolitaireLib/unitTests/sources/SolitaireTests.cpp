#include <algorithm>

#include "mock_ptr.h"
#include "Solitaire.h"
#include "archivers/Snapshot.h"
#include "cards/DeckGeneratorMock.h"
#include "cards/DeckGeneratorUtils.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/FoundationPileMock.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

namespace {
const PileId invalidFoundationPileId {4};
const PileId foundationPileId {3};
const PileId invalidTableauPileId {7};
const PileId tableauPileId {6};

const unsigned quantityToPullOut {2};

const auto deck {createSortedDeck()};
const Cards noCards;
}

MATCHER_P2(RangeEq, begin, end, "") {
    return std::equal(std::get<0>(arg), std::get<1>(arg), begin, end);
}

class SolitaireTest: public Test {
public:
    template <class T, std::size_t N>
    using SharedPtrArray = std::array<std::shared_ptr<T>, N>;

    template <class T, std::size_t N>
    SharedPtrArray<T, N> makeSharedPtrArray() {
        SharedPtrArray<T, N> result;
        for (auto& ptr: result)
            ptr = std::make_shared<T>();
        return result;
    }

    template <class CopiedArrayType, class T, std::size_t N>
    SharedPtrArray<CopiedArrayType, N> copySharedPtrArray(
        const SharedPtrArray<T, N>& source)
    {
        SharedPtrArray<CopiedArrayType, N> copy;
        std::copy(source.begin(), source.end(), copy.begin());
        return copy;
    }

    mock_ptr<DeckGeneratorMock> deckGeneratorMock;
    std::shared_ptr<StockPileMock> stockPileMock {std::make_shared<StockPileMock>()};

    std::array<std::shared_ptr<FoundationPileMock>, 4>
        foundationPileMocks {makeSharedPtrArray<FoundationPileMock, 4>()};

    std::array<std::shared_ptr<TableauPileMock>, 7>
        tableauPileMocks {makeSharedPtrArray<TableauPileMock, 7>()};

    Solitaire solitaire {
        deckGeneratorMock.make_unique(),
        stockPileMock,
        copySharedPtrArray<FoundationPile>(foundationPileMocks),
        copySharedPtrArray<TableauPile>(tableauPileMocks)
    };

    void expectTableauPilesInitialization(const Deck& deck) {
        EXPECT_CALL(*tableauPileMocks[0], initialize(_, _))
            .With(AllArgs(RangeEq(deck.begin(), std::next(deck.begin()))));
        EXPECT_CALL(*tableauPileMocks[1], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin()), std::next(deck.begin(), 3))));
        EXPECT_CALL(*tableauPileMocks[2], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 3), std::next(deck.begin(), 6))));
        EXPECT_CALL(*tableauPileMocks[3], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 6), std::next(deck.begin(), 10))));
        EXPECT_CALL(*tableauPileMocks[4], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 10), std::next(deck.begin(), 15))));
        EXPECT_CALL(*tableauPileMocks[5], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 15), std::next(deck.begin(), 21))));
        EXPECT_CALL(*tableauPileMocks[6], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 21), std::next(deck.begin(), 28))));
    }
};

TEST_F(SolitaireTest, afterInitializationHandIsEmpty) {
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireTest, tryGetFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(
        solitaire.getFoundationPile(invalidFoundationPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, getFoundationPile) {
    EXPECT_EQ(
        &solitaire.getFoundationPile(foundationPileId),
        foundationPileMocks[foundationPileId].get()
    );
}

TEST_F(SolitaireTest, tryGetTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(solitaire.getTableauPile(invalidTableauPileId), std::runtime_error);
}

TEST_F(SolitaireTest, getTableauPile) {
    EXPECT_EQ(
        &solitaire.getTableauPile(tableauPileId),
        tableauPileMocks[tableauPileId].get()
    );
}

TEST_F(SolitaireTest, getStockPile) {
    EXPECT_EQ(&solitaire.getStockPile(), stockPileMock.get());
}

TEST_F(SolitaireTest, tryPullOutCardFromFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(
        solitaire.tryPullOutCardFromFoundationPile(invalidFoundationPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, tryPullOutNoCardsFromFoundationPileWhenHandIsEmpty) {
    EXPECT_CALL(*foundationPileMocks[foundationPileId], tryPullOutCard())
        .WillOnce(Return(std::nullopt));

    solitaire.tryPullOutCardFromFoundationPile(foundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireTest, tryPullOutCardFromFoundationPileWhenHandIsEmpty) {
    Cards cardsInHand {deck.back()};

    EXPECT_CALL(*foundationPileMocks[foundationPileId], tryPullOutCard())
        .WillOnce(Return(deck.back()));

    solitaire.tryPullOutCardFromFoundationPile(foundationPileId);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(cardsInHand));
}

TEST_F(SolitaireTest, tryPullOutCardsFromTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(
        solitaire.tryPullOutCardsFromTableauPile(invalidTableauPileId, quantityToPullOut),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, tryPullOutNoCardsFromTableauPileWhenHandIsEmpty) {
    EXPECT_CALL(*tableauPileMocks[tableauPileId], tryPullOutCards(quantityToPullOut))
        .WillOnce(Return(noCards));

    solitaire.tryPullOutCardsFromTableauPile(tableauPileId, quantityToPullOut);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireTest, tryPullOutCardsFromTableauPileWhenHandIsEmpty) {
    Cards cards {
        Card {Value::Five, Suit::Spade},
        Card {Value::Four, Suit::Diamond}
    };

    EXPECT_CALL(*tableauPileMocks[tableauPileId], tryPullOutCards(quantityToPullOut))
        .WillOnce(Return(cards));

    solitaire.tryPullOutCardsFromTableauPile(tableauPileId, quantityToPullOut);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(cards));
}

TEST_F(SolitaireTest, tryPullOutNoCardsFromStockPileWhenHandIsEmpty) {
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(std::nullopt));
    solitaire.tryPullOutCardFromStockPile();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireTest, tryPullOutCardFromStockPileWhenHandIsEmpty) {
    Cards cardsInHand {deck.back()};
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(deck.back()));
    solitaire.tryPullOutCardFromStockPile();
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(cardsInHand));
}

TEST_F(SolitaireTest, onNewGameGenerateAndDistributeCards) {
    EXPECT_CALL(*deckGeneratorMock, generate()).WillOnce(Return(deck));

    for (auto& pile: foundationPileMocks)
        EXPECT_CALL(*pile, initialize());

    expectTableauPilesInitialization(deck);

    EXPECT_CALL(*stockPileMock, initialize(_, _))
        .With(AllArgs(RangeEq(std::next(deck.begin(), 28), deck.end())));

    solitaire.startNewGame();
}

}
