#include <algorithm>

#include "mock_ptr.h"
#include "Solitaire.h"
#include "archivers/HistoryTrackerMock.h"
#include "archivers/MoveCardsOperationSnapshotCreatorMock.h"
#include "archivers/SnapshotMock.h"
#include "cards/DeckGeneratorMock.h"
#include "cards/DeckGeneratorUtils.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "piles/FoundationPileMock.h"
#include "piles/PileId.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::piles;
using namespace solitaire::piles::interfaces;

namespace solitaire {

namespace {
constexpr unsigned foundationPilesCount {4};
constexpr unsigned tableauPilesCount {7};

const PileId invalidFoundationPileId {foundationPilesCount};
const PileId lastFoundationPileId {foundationPilesCount - 1};
const PileId invalidTableauPileId {tableauPilesCount};
const PileId lastTableauPileId {tableauPilesCount - 1};

constexpr unsigned quantityToPullOut {2};

const auto deck {createSortedDeck()};
const Cards noCards;
const Cards oneCard {deck.back()};
const Cards twoCards {deck[0], deck[1]};

MATCHER_P2(RangeEq, begin, end, "") {
    return std::equal(std::get<0>(arg), std::get<1>(arg), begin, end);
}
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

    std::array<std::shared_ptr<FoundationPileMock>, foundationPilesCount>
        foundationPileMocks {
            makeSharedPtrArray<FoundationPileMock, foundationPilesCount>()};

    std::array<std::shared_ptr<TableauPileMock>, tableauPilesCount>
        tableauPileMocks {
            makeSharedPtrArray<TableauPileMock, tableauPilesCount>()};

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
    using StrictSnapshotMockPtr = mock_ptr<StrictMock<SnapshotMock>>;

    SolitaireEmptyHandTest() {
        EXPECT_CALL(*foundationPileMocks[0], getTopCardValue()).WillRepeatedly(Return(Value::King));
        EXPECT_CALL(*foundationPileMocks[1], getTopCardValue()).WillRepeatedly(Return(Value::King));
        EXPECT_CALL(*foundationPileMocks[2], getTopCardValue()).WillRepeatedly(Return(Value::King));
        EXPECT_CALL(*foundationPileMocks[3], getTopCardValue()).WillRepeatedly(Return(Value::Queen));
    }

    void ignoreEmptyHandPossibleOperationsTest() {
        EXPECT_CALL(*historyTrackerMock, getHistorySize()).Times(0);
        EXPECT_CALL(*historyTrackerMock, undo()).Times(0);
        EXPECT_CALL(lastFoundationPileMock, tryPullOutCard()).Times(0);
        EXPECT_CALL(lastTableauPileMock, isTopCardCovered()).Times(0);
        EXPECT_CALL(lastTableauPileMock, tryUncoverTopCard()).Times(0);
        EXPECT_CALL(lastTableauPileMock, tryPullOutCards(_)).Times(0);
        EXPECT_CALL(*stockPileMock, getCards()).Times(0);
        EXPECT_CALL(*stockPileMock, trySelectNextCard()).Times(0);
        EXPECT_CALL(*stockPileMock, tryPullOutCard()).Times(0);

        solitaire.tryUndoOperation();
        solitaire.tryPullOutCardFromFoundationPile(lastFoundationPileId);
        solitaire.tryUncoverTableauPileTopCard(lastTableauPileId);
        solitaire.tryPullOutCardsFromTableauPile(lastTableauPileId, quantityToPullOut);
        solitaire.trySelectNextStockPileCard();
        solitaire.tryPullOutCardFromStockPile();
    }

    void finishGame() {
        for (auto& pile: foundationPileMocks)
            EXPECT_CALL(*pile, getTopCardValue()).WillRepeatedly(Return(Value::King));
    }

    template <class Pile>
    void expectTryingAddCardAndClearIt(Pile& pile, std::optional<Card>& card) {
        EXPECT_CALL(pile, tryAddCard(card))
            .WillOnce(Invoke([](auto& card) {card = std::nullopt;}));
    }

    template <class Pile>
    void expectTryingAddCardsAndClearThem(Pile& pile, Cards& cards) {
        EXPECT_CALL(pile, tryAddCards(cards))
            .WillOnce(Invoke([](auto& cards) {cards.clear();}));
    }

    template <class Pile>
    void expectSnapshotCreation(Pile& pile, StrictSnapshotMockPtr& snapshot) {
        EXPECT_CALL(pile, createSnapshot())
            .WillOnce(Return(ByMove(snapshot.make_unique())));
    }

    void expectSavingSourcePileSnapshot(const StrictSnapshotMockPtr& snapshotMock)
    {
        EXPECT_CALL(
            *moveCardsOperationSnapshotCreatorMock,
            saveSourcePileSnapshot(Pointer(snapshotMock.get()))
        );
    }

    auto& expectMoveOperationSnapshotCreation(
        const StrictSnapshotMockPtr& passedSnapshot)
    {
        return EXPECT_CALL(
            *moveCardsOperationSnapshotCreatorMock,
            createSnapshotIfCardsMovedToOtherPile(Pointer(passedSnapshot.get()))
        );
    }

    StrictSnapshotMockPtr snapshotMock;
};

TEST_F(SolitaireEmptyHandTest, dontUndoOperationWhenHistoryIsEmpty) {
    EXPECT_CALL(*historyTrackerMock, getHistorySize()).WillOnce(Return(0));
    EXPECT_CALL(*historyTrackerMock, undo()).Times(0);
    solitaire.tryUndoOperation();
}

TEST_F(SolitaireEmptyHandTest, undoOperationWhenHistoryIsNotEmpty) {
    EXPECT_CALL(*historyTrackerMock, getHistorySize()).WillOnce(Return(1));
    EXPECT_CALL(*historyTrackerMock, undo());
    solitaire.tryUndoOperation();
}

TEST_F(SolitaireEmptyHandTest, ignoreTryOfPuttingCardBack) {
    EXPECT_CALL(*moveCardsOperationSnapshotCreatorMock, restoreSourcePile()).Times(0);
    solitaire.tryPutCardsBackFromHand();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, tryPullOutNoCardsFromFoundationPile) {
    EXPECT_CALL(lastFoundationPileMock, createSnapshot());
    EXPECT_CALL(lastFoundationPileMock, tryPullOutCard())
        .WillOnce(Return(std::nullopt));

    solitaire.tryPullOutCardFromFoundationPile(lastFoundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireEmptyHandTest, tryPullOutCardFromFoundationPile) {
    InSequence seq;
    expectSnapshotCreation(lastFoundationPileMock, snapshotMock);
    EXPECT_CALL(lastFoundationPileMock, tryPullOutCard())
        .WillOnce(Return(oneCard.back()));
    expectSavingSourcePileSnapshot(snapshotMock);

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
    expectSnapshotCreation(lastTableauPileMock, snapshotMock);
    EXPECT_CALL(lastTableauPileMock, tryUncoverTopCard());
    EXPECT_CALL(*historyTrackerMock, save(Pointer(snapshotMock.get())));

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
    expectSnapshotCreation(lastTableauPileMock, snapshotMock);
    EXPECT_CALL(lastTableauPileMock, tryPullOutCards(quantityToPullOut))
        .WillOnce(Return(twoCards));
    expectSavingSourcePileSnapshot(snapshotMock);

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
    expectSnapshotCreation(*stockPileMock, snapshotMock);
    EXPECT_CALL(*stockPileMock, trySelectNextCard());
    EXPECT_CALL(*historyTrackerMock, save(Pointer(snapshotMock.get())));

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
    expectSnapshotCreation(*stockPileMock, snapshotMock);
    EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(oneCard.back()));
    expectSavingSourcePileSnapshot(snapshotMock);

    solitaire.tryPullOutCardFromStockPile();
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(oneCard));
}

TEST_F(SolitaireEmptyHandTest, gameIsNotFinishedWhenAnyOfFoundationPilesDoesNotHaveKingOnTop)
{
    EXPECT_CALL(*foundationPileMocks[0], getTopCardValue()).WillOnce(Return(Value::King));
    EXPECT_CALL(*foundationPileMocks[1], getTopCardValue()).WillOnce(Return(Value::King));
    EXPECT_CALL(*foundationPileMocks[2], getTopCardValue()).WillOnce(Return(Value::King));
    EXPECT_CALL(*foundationPileMocks[3], getTopCardValue()).WillOnce(Return(Value::Queen));

    EXPECT_FALSE(solitaire.isGameFinished());
}

TEST_F(SolitaireEmptyHandTest, gameIsFinishedWhenAllOfFoundationPilesHaveKingOnTop) {
    for (auto& pile: foundationPileMocks)
        EXPECT_CALL(*pile, getTopCardValue()).WillOnce(Return(Value::King));

    EXPECT_TRUE(solitaire.isGameFinished());
}

TEST_F(SolitaireEmptyHandTest, ignoreSomeOperationsIfGameFinished) {
    finishGame();
    ignoreEmptyHandPossibleOperationsTest();
}

class SolitaireHandWithOneCardTest: public SolitaireEmptyHandTest {
public:
    SolitaireHandWithOneCardTest() {
        expectSnapshotCreation(*stockPileMock, snapshotMock);
        EXPECT_CALL(*stockPileMock, tryPullOutCard()).WillOnce(Return(oneCard.back()));
        expectSavingSourcePileSnapshot(snapshotMock);

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
    for (auto& pile: foundationPileMocks)
        EXPECT_CALL(*pile, getTopCardValue()).Times(0);

    EXPECT_CALL(*deckGeneratorMock, generate()).WillOnce(Return(deck));
    expectFoundationPilesInitialization();
    expectTableauPilesInitialization();
    expectStockPileInitialization();

    solitaire.startNewGame();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireHandWithOneCardTest, tryPutCardsBack) {
    EXPECT_CALL(*moveCardsOperationSnapshotCreatorMock, restoreSourcePile());
    solitaire.tryPutCardsBackFromHand();
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireHandWithOneCardTest, ignoreSomeOperationsIfHandNotEmpty) {
    ignoreEmptyHandPossibleOperationsTest();
}

TEST_F(SolitaireHandWithOneCardTest, ignoreSomeOperationsIfGameFinished) {
    finishGame();

    EXPECT_CALL(*moveCardsOperationSnapshotCreatorMock, restoreSourcePile()).Times(0);
    EXPECT_CALL(lastFoundationPileMock, tryAddCard(_)).Times(0);
    EXPECT_CALL(lastTableauPileMock, tryAddCards(_)).Times(0);

    solitaire.tryPutCardsBackFromHand();
    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    solitaire.tryAddCardsOnTableauPile(lastTableauPileId);
}

class SolitaireHandWithOneCardAddCardTest: public SolitaireHandWithOneCardTest
{
public:
    StrictSnapshotMockPtr snapshotMock2;
    StrictSnapshotMockPtr moveOperationSnapshot;
};

TEST_F(SolitaireHandWithOneCardAddCardTest, tryAddCardOnFoundationPileWithFail) {
    std::optional<Card> cardToAdd {oneCard.back()};

    EXPECT_CALL(lastFoundationPileMock, createSnapshot());
    EXPECT_CALL(lastFoundationPileMock, tryAddCard(cardToAdd));

    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(oneCard));
}

TEST_F(SolitaireHandWithOneCardAddCardTest, addCardOnFoundationPileWithoutHistoryRecord) {
    std::optional<Card> cardToAdd {oneCard.back()};

    InSequence seq;
    expectSnapshotCreation(lastFoundationPileMock, snapshotMock2);
    expectTryingAddCardAndClearIt(lastFoundationPileMock, cardToAdd);
    expectMoveOperationSnapshotCreation(snapshotMock2).WillOnce(ReturnNull());
    EXPECT_CALL(*historyTrackerMock, save(_)).Times(0);

    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireHandWithOneCardAddCardTest, addCardOnFoundationPileWithHistoryRecord) {
    std::optional<Card> cardToAdd {oneCard.back()};

    InSequence seq;
    expectSnapshotCreation(lastFoundationPileMock, snapshotMock2);
    expectTryingAddCardAndClearIt(lastFoundationPileMock, cardToAdd);
    expectMoveOperationSnapshotCreation(snapshotMock2)
        .WillOnce(Return(ByMove(moveOperationSnapshot.make_unique())));
    EXPECT_CALL(*historyTrackerMock, save(Pointer(moveOperationSnapshot.get())));

    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

class SolitaireHandWithTwoCardsTest: public SolitaireEmptyHandTest {
public:
    SolitaireHandWithTwoCardsTest() {
        expectSnapshotCreation(lastTableauPileMock, snapshotMock);
        EXPECT_CALL(lastTableauPileMock, tryPullOutCards(quantityToPullOut))
            .WillOnce(Return(twoCards));
        expectSavingSourcePileSnapshot(snapshotMock);

        solitaire.tryPullOutCardsFromTableauPile(lastTableauPileId, quantityToPullOut);
    }

    StrictSnapshotMockPtr snapshotMock2;
    StrictSnapshotMockPtr moveOperationSnapshot;
};

TEST_F(SolitaireHandWithTwoCardsTest, ignoreTryOfAddingCardOnFoundationPile) {
    EXPECT_CALL(lastFoundationPileMock, createSnapshot()).Times(0);
    EXPECT_CALL(lastFoundationPileMock, tryAddCard(_)).Times(0);

    solitaire.tryAddCardOnFoundationPile(lastFoundationPileId);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(twoCards));
}

TEST_F(SolitaireHandWithTwoCardsTest, tryAddCardOnTableauPileWithFail) {
    auto cardsToAdd {twoCards};

    EXPECT_CALL(lastTableauPileMock, createSnapshot());
    EXPECT_CALL(lastTableauPileMock, tryAddCards(cardsToAdd));

    solitaire.tryAddCardsOnTableauPile(lastTableauPileId);
    EXPECT_THAT(solitaire.getCardsInHand(), ContainerEq(twoCards));
}

TEST_F(SolitaireHandWithTwoCardsTest, addCardOnTableauPileWithoutHistoryRecord) {
    auto cardsToAdd {twoCards};

    InSequence seq;
    expectSnapshotCreation(lastTableauPileMock, snapshotMock2);
    expectTryingAddCardsAndClearThem(lastTableauPileMock, cardsToAdd);
    expectMoveOperationSnapshotCreation(snapshotMock2).WillOnce(ReturnNull());
    EXPECT_CALL(*historyTrackerMock, save(_)).Times(0);

    solitaire.tryAddCardsOnTableauPile(lastTableauPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

TEST_F(SolitaireHandWithTwoCardsTest, addCardOnTableauPileWithHistoryRecord) {
    auto cardsToAdd {twoCards};

    InSequence seq;
    expectSnapshotCreation(lastTableauPileMock, snapshotMock2);
    expectTryingAddCardsAndClearThem(lastTableauPileMock, cardsToAdd);
    expectMoveOperationSnapshotCreation(snapshotMock2)
        .WillOnce(Return(ByMove(moveOperationSnapshot.make_unique())));
    EXPECT_CALL(*historyTrackerMock, save(Pointer(moveOperationSnapshot.get())));

    solitaire.tryAddCardsOnTableauPile(lastTableauPileId);
    EXPECT_TRUE(solitaire.getCardsInHand().empty());
}

}
