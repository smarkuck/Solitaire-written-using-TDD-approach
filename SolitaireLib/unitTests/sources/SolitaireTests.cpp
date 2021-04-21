#include <algorithm>

#include "mock_ptr.h"
#include "cards/Card.h"
#include "Solitaire.h"
#include "archivers/HistoryTrackerMock.h"
#include "archivers/MoveCardsOperationSnapshotCreatorMock.h"
#include "archivers/SnapshotMock.h"
#include "cards/DeckGeneratorMock.h"
#include "cards/DeckGeneratorUtils.h"
#include "gmock/gmock.h"
#include "piles/FoundationPileMock.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

namespace {
const PileId invalidFoundationPileId {4};
const PileId lastFoundationPileId {3};
const PileId invalidTableauPileId {7};
const PileId lastTableauPileId {6};

const unsigned quantityToPullOut {2};

const auto deck {createSortedDeck()};
const Cards noCards;
const Cards oneCard {deck.back()};
const Cards twoCards {deck[0], deck[1]};
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

    mock_ptr<StrictMock<HistoryTrackerMock>> historyTrackerMock;
    mock_ptr<StrictMock<MoveCardsOperationSnapshotCreatorMock>>
        moveCardsOperationSnapshotCreatorMock;

    FoundationPileMock& lastFoundationPileMock {
        *foundationPileMocks[lastFoundationPileId]};
    TableauPileMock& lastTableauPileMock {
        *tableauPileMocks[lastTableauPileId]};

    Solitaire solitaire {
        deckGeneratorMock.make_unique(),
        stockPileMock,
        copySharedPtrArray<FoundationPile>(foundationPileMocks),
        copySharedPtrArray<TableauPile>(tableauPileMocks),
        historyTrackerMock.make_unique(),
        moveCardsOperationSnapshotCreatorMock.make_unique()
    };
};

TEST_F(SolitaireTest, afterInitializationHandIsEmpty) {
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireTest, tryPullOutCardFromFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(
        solitaire.tryPullOutCardFromFoundationPile(invalidFoundationPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, tryAddCardOnFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(
        solitaire.tryAddCardOnFoundationPile(invalidFoundationPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, getFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(
        solitaire.getFoundationPile(invalidFoundationPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, getFoundationPile) {
    EXPECT_EQ(
        &solitaire.getFoundationPile(lastFoundationPileId),
        &lastFoundationPileMock
    );
}

TEST_F(SolitaireTest, tryUncoverTableauPileTopCardWithIdGreaterThanSix) {
    EXPECT_THROW(
        solitaire.tryUncoverTableauPileTopCard(invalidTableauPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, tryPullOutCardsFromTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(
        solitaire.tryPullOutCardsFromTableauPile(invalidTableauPileId, quantityToPullOut),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, tryAddCardsOnTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(
        solitaire.tryAddCardsOnTableauPile(invalidTableauPileId),
        std::runtime_error
    );
}

TEST_F(SolitaireTest, getTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(solitaire.getTableauPile(invalidTableauPileId), std::runtime_error);
}

TEST_F(SolitaireTest, getTableauPile) {
    EXPECT_EQ(&solitaire.getTableauPile(lastTableauPileId), &lastTableauPileMock);
}

TEST_F(SolitaireTest, getStockPile) {
    EXPECT_EQ(&solitaire.getStockPile(), stockPileMock.get());
}

class SolitaireEmptyHandTest: public SolitaireTest {
public:
    void expectSavingSnapshotMock() {
        EXPECT_CALL(
            *moveCardsOperationSnapshotCreatorMock,
            saveSourcePileSnapshot(Pointer(&*snapshotMock))
        );
    }

    mock_ptr<StrictMock<SnapshotMock>> snapshotMock;
};

TEST_F(SolitaireEmptyHandTest, tryPullOutNoCardsFromFoundationPile) {
    EXPECT_CALL(lastFoundationPileMock, createSnapshot());
    EXPECT_CALL(lastFoundationPileMock, tryPullOutCard())
        .WillOnce(Return(std::nullopt));

    solitaire.tryPullOutCardFromFoundationPile(lastFoundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, tryPullOutCardFromFoundationPile) {
    InSequence seq;
    EXPECT_CALL(lastFoundationPileMock, createSnapshot())
        .WillOnce(Return(ByMove(snapshotMock.make_unique())));
    EXPECT_CALL(lastFoundationPileMock, tryPullOutCard())
        .WillOnce(Return(oneCard.back()));
    expectSavingSnapshotMock();

    solitaire.tryPullOutCardFromFoundationPile(lastFoundationPileId);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(oneCard));
}

TEST_F(SolitaireEmptyHandTest, tryAddCardOnFoundationPile) {
    EXPECT_CALL(lastFoundationPileMock, createSnapshot()).Times(0);
    EXPECT_CALL(lastFoundationPileMock, tryAddCard(_)).Times(0);

    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, dontUncoverTableauPileTopCardWhenTopCardUncovered) {
    EXPECT_CALL(lastTableauPileMock, isTopCardCovered()).WillOnce(Return(false));
    EXPECT_CALL(lastTableauPileMock, createSnapshot()).Times(0);
    EXPECT_CALL(lastTableauPileMock, tryUncoverTopCard()).Times(0);

    solitaire.tryUncoverTableauPileTopCard(lastTableauPileId);
}

TEST_F(SolitaireEmptyHandTest, tryUncoverTableauPileTopCard) {
    InSequence seq;
    EXPECT_CALL(lastTableauPileMock, isTopCardCovered()).WillOnce(Return(true));
    EXPECT_CALL(lastTableauPileMock, createSnapshot())
        .WillOnce(Return(ByMove(snapshotMock.make_unique())));
    EXPECT_CALL(lastTableauPileMock, tryUncoverTopCard());
    EXPECT_CALL(*historyTrackerMock, save(Pointer(&*snapshotMock)));

    solitaire.tryUncoverTableauPileTopCard(lastTableauPileId);
}

TEST_F(SolitaireEmptyHandTest, tryPullOutNoCardsFromTableauPile) {
    EXPECT_CALL(lastTableauPileMock, createSnapshot());
    EXPECT_CALL(lastTableauPileMock, tryPullOutCards(quantityToPullOut))
        .WillOnce(Return(noCards));

    solitaire.tryPullOutCardsFromTableauPile(lastTableauPileId, quantityToPullOut);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, tryPullOutCardsFromTableauPile) {
    InSequence seq;
    EXPECT_CALL(lastTableauPileMock, createSnapshot())
        .WillOnce(Return(ByMove(snapshotMock.make_unique())));
    EXPECT_CALL(lastTableauPileMock, tryPullOutCards(quantityToPullOut))
        .WillOnce(Return(twoCards));
    expectSavingSnapshotMock();

    solitaire.tryPullOutCardsFromTableauPile(lastTableauPileId, quantityToPullOut);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(twoCards));
}

TEST_F(SolitaireEmptyHandTest, tryAddCardsOnTableauPile) {
    EXPECT_CALL(lastTableauPileMock, createSnapshot()).Times(0);
    EXPECT_CALL(lastTableauPileMock, tryAddCards(_)).Times(0);

    solitaire.tryAddCardsOnTableauPile(lastTableauPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, dontSelectNextStockPileCardWhenNoCards) {
    EXPECT_CALL(*stockPileMock, getCards()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(*stockPileMock, createSnapshot()).Times(0);
    EXPECT_CALL(*stockPileMock, trySelectNextCard()).Times(0);
    solitaire.trySelectNextStockPileCard();
}

TEST_F(SolitaireEmptyHandTest, trySelectNextStockPileCard) {
    InSequence seq;
    EXPECT_CALL(*stockPileMock, getCards()).WillOnce(ReturnRef(oneCard));
    EXPECT_CALL(*stockPileMock, createSnapshot())
        .WillOnce(Return(ByMove(snapshotMock.make_unique())));
    EXPECT_CALL(*stockPileMock, trySelectNextCard());
    EXPECT_CALL(*historyTrackerMock, save(Pointer(&*snapshotMock)));

    solitaire.trySelectNextStockPileCard();
}

TEST_F(SolitaireEmptyHandTest, tryPullOutNoCardsFromStockPile) {
    EXPECT_CALL(*stockPileMock, createSnapshot());
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(std::nullopt));

    solitaire.tryPullOutCardFromStockPile();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, tryPullOutCardFromStockPile) {
    InSequence seq;
    EXPECT_CALL(*stockPileMock, createSnapshot())
        .WillOnce(Return(ByMove(snapshotMock.make_unique())));
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(oneCard.back()));
    expectSavingSnapshotMock();

    solitaire.tryPullOutCardFromStockPile();
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(oneCard));
}

class SolitaireHandWithOneCardTest: public SolitaireEmptyHandTest {
public:
    SolitaireHandWithOneCardTest() {
        EXPECT_CALL(*stockPileMock, createSnapshot());
        EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(deck.back()));
        EXPECT_CALL(*moveCardsOperationSnapshotCreatorMock, saveSourcePileSnapshot(_));
        solitaire.tryPullOutCardFromStockPile();
    }

    void expectFoundationPilesInitialization() {
        for (auto& pile: foundationPileMocks)
            EXPECT_CALL(*pile, initialize());
    }

    void expectTableauPilesInitialization() {
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

    void expectStockPileInitialization() {
        EXPECT_CALL(*stockPileMock, initialize(_, _))
            .With(AllArgs(RangeEq(std::next(deck.begin(), 28), deck.end())));
    }
};

TEST_F(SolitaireHandWithOneCardTest, onNewGameClearHandAndDistributeCards) {
    EXPECT_CALL(*deckGeneratorMock, generate()).WillOnce(Return(deck));
    expectFoundationPilesInitialization();
    expectTableauPilesInitialization();
    expectStockPileInitialization();

    solitaire.startNewGame();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireHandWithOneCardTest, ignoreSomeOperationsIfHandNotEmpty) {
    EXPECT_CALL(lastFoundationPileMock, tryPullOutCard()).Times(0);
    EXPECT_CALL(lastTableauPileMock, tryUncoverTopCard()).Times(0);
    EXPECT_CALL(lastTableauPileMock, tryPullOutCards(_)).Times(0);
    EXPECT_CALL(*stockPileMock, trySelectNextCard()).Times(0);
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).Times(0);

    solitaire.tryPullOutCardFromFoundationPile(lastFoundationPileId);
    solitaire.tryUncoverTableauPileTopCard(lastTableauPileId);
    solitaire.tryPullOutCardsFromTableauPile(lastTableauPileId, quantityToPullOut);
    solitaire.trySelectNextStockPileCard();
    solitaire.tryPullOutCardFromStockPile();
}

}
