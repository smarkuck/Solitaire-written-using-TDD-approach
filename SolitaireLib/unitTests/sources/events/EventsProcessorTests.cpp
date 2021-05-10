#include "ButtonMock.h"
#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "colliders/FoundationPileColliderMock.h"
#include "colliders/StockPileColliderMock.h"
#include "colliders/TableauPileColliderMock.h"
#include "events/EventsDefinitions.h"
#include "events/EventsProcessor.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::colliders;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::events {

namespace {
const Cards noCards;
const Cards cards {5};

constexpr unsigned foundationPilesCount = 4;
constexpr unsigned tableauPilesCount = 7;
constexpr unsigned middleTableauPileCardIndex = 2;
const unsigned lastTableauPileCardIndex = cards.size() - 1;

const PileId lastFoundationPileId {foundationPilesCount - 1};
const PileId beforeLastFoundationPileId {lastFoundationPileId - 1};
const PileId lastTableauPileId {tableauPilesCount - 1};
const PileId beforeLastTableauPileId {lastTableauPileId - 1};

constexpr Position mousePosition {36, 12};
constexpr Position cardsInHandPosition {46, 26};
constexpr Position pilePosition {45, 93};

constexpr MouseLeftButtonDown mouseLeftButtonDownEvent {Position {24, 51}};
constexpr MouseMove mouseMoveEvent {Position {99, 13}};
}

class EventsProcessorTests: public Test {
public:
    void expectEvent(const Event& event) {
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(event));
    }

    void ignoreLeftButtonDownOnButtons() {
        EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
        EXPECT_CALL(buttonMock, collidesWith(_)).WillOnce(Return(false));
        EXPECT_CALL(contextMock, getUndoButton()).WillOnce(ReturnRef(buttonMock));
        EXPECT_CALL(buttonMock, collidesWith(_)).WillOnce(Return(false));
    }

    void ignoreLeftButtonDownOnFoundationPilesFromFirstTo(unsigned pilesToIgnore) {
        for (PileId id {0}; id < pilesToIgnore; ++id) {
            EXPECT_CALL(contextMock, getFoundationPileCollider(id))
                .WillOnce(ReturnRef(foundationPileColliderMock));
            EXPECT_CALL(foundationPileColliderMock, collidesWithPoint(_))
                .WillOnce(Return(false));
        }
    }

    void ignoreLeftButtonDownOnTableauPilesFromFirstTo(unsigned pilesToIgnore) {
        for (PileId id {0}; id < pilesToIgnore; ++id) {
            EXPECT_CALL(contextMock, getTableauPileCollider(id))
                .WillOnce(ReturnRef(tableauPileColliderMock));
            EXPECT_CALL(tableauPileColliderMock, tryGetCollidedCardIndex(_))
                .WillOnce(Return(std::nullopt));
        }
    }

    void ignoreLeftButtonDownOnStockPile() {
        expectGetStockPileCollider();
        EXPECT_CALL(stockPileColliderMock, coveredCardsCollidesWith(_))
            .WillOnce(Return(false));
        EXPECT_CALL(stockPileColliderMock, uncoveredCardsCollidesWith(_))
            .WillOnce(Return(false));
    }

    void ignoreLeftButtonUpOnFoundationPilesFromFirstTo(unsigned pilesToIgnore) {
        for (PileId id {0}; id < pilesToIgnore; ++id) {
            EXPECT_CALL(contextMock, getFoundationPileCollider(id))
                .WillOnce(ReturnRef(foundationPileColliderMock));
            EXPECT_CALL(foundationPileColliderMock, collidesWithCardsInHand(_))
                .WillOnce(Return(false));
        }
    }

    void ignoreLeftButtonUpOnTableauPilesFromFirstTo(unsigned pilesToIgnore) {
        for (PileId id {0}; id < pilesToIgnore; ++id) {
            EXPECT_CALL(contextMock, getTableauPileCollider(id))
                .WillOnce(ReturnRef(tableauPileColliderMock));
            EXPECT_CALL(tableauPileColliderMock, collidesWithCardsInHand(_))
                .WillOnce(Return(false));
        }
    }

    void acceptLeftButtonDownOnFoundationPile(
        const PileId id, const MouseLeftButtonDown& event)
    {
        EXPECT_CALL(contextMock, getFoundationPileCollider(id))
            .WillOnce(ReturnRef(foundationPileColliderMock));
        EXPECT_CALL(foundationPileColliderMock, collidesWithPoint(event.position))
            .WillOnce(Return(true));
    }

    void acceptLeftButtonDownOnTableauPileCard(
        const PileId id, const unsigned cardIndex, const MouseLeftButtonDown& event)
    {
        EXPECT_CALL(contextMock, getTableauPileCollider(id))
            .WillOnce(ReturnRef(tableauPileColliderMock));
        EXPECT_CALL(tableauPileColliderMock, tryGetCollidedCardIndex(event.position))
            .WillOnce(Return(cardIndex));
    }

    void acceptLeftButtonDownOnCoveredStockPileCards(const MouseLeftButtonDown& event) {
        EXPECT_CALL(stockPileColliderMock, coveredCardsCollidesWith(event.position))
            .WillOnce(Return(true));
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, trySelectNextStockPileCard());
    }

    void acceptLeftButtonDownOnUncoveredStockPileCards(const MouseLeftButtonDown& event) {
        EXPECT_CALL(stockPileColliderMock, coveredCardsCollidesWith(event.position))
            .WillOnce(Return(false));
        EXPECT_CALL(stockPileColliderMock, uncoveredCardsCollidesWith(event.position))
            .WillOnce(Return(true));
        EXPECT_CALL(contextMock, setMousePosition(event.position));
        EXPECT_CALL(stockPileColliderMock, getUncoveredCardsPosition())
            .WillOnce(Return(pilePosition));
        EXPECT_CALL(contextMock, setCardsInHandPosition(pilePosition));
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, tryPullOutCardFromStockPile());
    }

    void acceptLeftButtonUpOnFoundationPile(
        const PileId id, const Position& cardsInHandPosition)
    {
        EXPECT_CALL(contextMock, getFoundationPileCollider(id))
            .WillOnce(ReturnRef(foundationPileColliderMock));
        EXPECT_CALL(
            foundationPileColliderMock,
            collidesWithCardsInHand(cardsInHandPosition)
        ).WillOnce(Return(true));
    }

    void acceptLeftButtonUpOnTableauPile(
        const PileId id, const Position& cardsInHandPosition)
    {
        EXPECT_CALL(contextMock, getTableauPileCollider(id))
            .WillOnce(ReturnRef(tableauPileColliderMock));
        EXPECT_CALL(
            tableauPileColliderMock,
            collidesWithCardsInHand(cardsInHandPosition)
        ).WillOnce(Return(true));
    }

    void expectGetTableauPileAndItsCollider(const PileId id) {
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getTableauPile(id))
            .WillOnce(ReturnRef(tableauPileMock));
        EXPECT_CALL(contextMock, getTableauPileCollider(id))
            .WillOnce(ReturnRef(tableauPileColliderMock));
    }

    void expectTryPullOutCardsFromTableauPile(
        const MouseLeftButtonDown& event, const PileId id,
        const unsigned cardIndex, const unsigned cardsToPullout)
    {
        EXPECT_CALL(contextMock, setMousePosition(event.position));
        EXPECT_CALL(tableauPileColliderMock, getCardPosition(cardIndex))
            .WillOnce(Return(pilePosition));
        EXPECT_CALL(contextMock, setCardsInHandPosition(pilePosition));
        EXPECT_CALL(solitaireMock, tryPullOutCardsFromTableauPile(id, cardsToPullout));
    }

    void expectGetStockPileCollider() {
        EXPECT_CALL(contextMock, getStockPileCollider())
            .WillOnce(ReturnRef(stockPileColliderMock));
    }

    void expectMoveCardsInHand(const MouseMove& event) {
        EXPECT_CALL(contextMock, getMousePosition()).WillOnce(Return(mousePosition));
        EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
        EXPECT_CALL(contextMock, setMousePosition(event.position));
        const auto mouseMoveDelta = event.position - mousePosition;
        EXPECT_CALL(contextMock, setCardsInHandPosition(cardsInHandPosition + mouseMoveDelta));
    }

    InSequence seq;
    StrictMock<SolitaireMock> solitaireMock;
    StrictMock<FoundationPileColliderMock> foundationPileColliderMock;
    StrictMock<StockPileColliderMock> stockPileColliderMock;
    StrictMock<TableauPileColliderMock> tableauPileColliderMock;
    StrictMock<TableauPileMock> tableauPileMock;
    StrictMock<ButtonMock> buttonMock;
    StrictMock<ContextMock> contextMock;
    mock_ptr<StrictMock<EventsSourceMock>> eventsSourceMock;

    EventsProcessor eventsProcessor {
        contextMock, eventsSourceMock.make_unique()};
};

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingWhenNoEvents)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    ignoreLeftButtonDownOnStockPile();
    expectEvent(NoEvents {});
    eventsProcessor.processEvents();
    EXPECT_FALSE(eventsProcessor.shouldQuit());
}

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingOnQuitEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    ignoreLeftButtonDownOnStockPile();
    expectEvent(Quit {});
    eventsProcessor.processEvents();
    EXPECT_TRUE(eventsProcessor.shouldQuit());
}

TEST_F(EventsProcessorTests, startNewGameOnLeftButtonDownEvent) {
    expectEvent(mouseLeftButtonDownEvent);
    EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseLeftButtonDownEvent.position))
        .WillOnce(Return(true));
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, startNewGame());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, undoOperationOnLeftButtonDownEvent) {
    expectEvent(mouseLeftButtonDownEvent);
    EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseLeftButtonDownEvent.position))
        .WillOnce(Return(false));
    EXPECT_CALL(contextMock, getUndoButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseLeftButtonDownEvent.position))
        .WillOnce(Return(true));
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, tryUndoOperation());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryPullOutCardFromFoundationPileOnLeftButtonDownEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount - 1);
    acceptLeftButtonDownOnFoundationPile(lastFoundationPileId, mouseLeftButtonDownEvent);
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, tryPullOutCardFromFoundationPile(lastFoundationPileId));
    EXPECT_CALL(contextMock, setMousePosition(mouseLeftButtonDownEvent.position));
    EXPECT_CALL(foundationPileColliderMock, getPosition()).WillOnce(Return(pilePosition));
    EXPECT_CALL(contextMock, setCardsInHandPosition(pilePosition));
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryUncoverTableauPileTopCardOnLeftButtonDownEventOnTopCard)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount - 1);
    acceptLeftButtonDownOnTableauPileCard(
        lastTableauPileId, lastTableauPileCardIndex, mouseLeftButtonDownEvent);
    expectGetTableauPileAndItsCollider(lastTableauPileId);
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(cards));
    EXPECT_CALL(tableauPileMock, isTopCardCovered()).WillOnce(Return(true));
    EXPECT_CALL(solitaireMock, tryUncoverTableauPileTopCard(lastTableauPileId));
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryPullOutCardsFromTableauPileOnLeftButtonDownEventOnTopCard)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount - 1);
    acceptLeftButtonDownOnTableauPileCard(
        lastTableauPileId, lastTableauPileCardIndex, mouseLeftButtonDownEvent);
    expectGetTableauPileAndItsCollider(lastTableauPileId);
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(cards));
    EXPECT_CALL(tableauPileMock, isTopCardCovered()).WillOnce(Return(false));
    expectTryPullOutCardsFromTableauPile(
        mouseLeftButtonDownEvent, lastTableauPileId, lastTableauPileCardIndex, 1);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryPullOutCardsFromTableauPileOnLeftButtonDownEventOnNotTopCard)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount - 1);
    acceptLeftButtonDownOnTableauPileCard(
        lastTableauPileId, middleTableauPileCardIndex, mouseLeftButtonDownEvent);
    expectGetTableauPileAndItsCollider(lastTableauPileId);
    EXPECT_CALL(tableauPileMock, getCards()).WillOnce(ReturnRef(cards));
    expectTryPullOutCardsFromTableauPile(
        mouseLeftButtonDownEvent, lastTableauPileId,
        middleTableauPileCardIndex, cards.size() - middleTableauPileCardIndex);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       trySelectNextStockPileCardOnLeftButtonDownEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    expectGetStockPileCollider();
    acceptLeftButtonDownOnCoveredStockPileCards(mouseLeftButtonDownEvent);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryPullOutCardFromStockPileOnLeftButtonDownEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnButtons();
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    expectGetStockPileCollider();
    acceptLeftButtonDownOnUncoveredStockPileCards(mouseLeftButtonDownEvent);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, doNothingOnLeftButtonUpEventWhenHandIsEmpty) {
    expectEvent(MouseLeftButtonUp {});
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

class EventsProcessorLeftMouseButtonUpEventTests: public EventsProcessorTests {
public:
    EventsProcessorLeftMouseButtonUpEventTests() {
        expectEvent(MouseLeftButtonUp {});
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(cardsInHand));
        EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
    }

    void tryAddCardOnFoundationPileAndClearHand(const PileId id) {
        auto& cardsToClear = cardsInHand;
        EXPECT_CALL(solitaireMock, tryAddCardOnFoundationPile(id))
            .WillOnce(Invoke([&cardsToClear]() { cardsToClear.clear(); }));
    }

    void tryAddCardsOnTableauPileAndClearHand(const PileId id) {
        auto& cardsToClear = cardsInHand;
        EXPECT_CALL(solitaireMock, tryAddCardsOnTableauPile(id))
            .WillOnce(Invoke([&cardsToClear]() { cardsToClear.clear(); }));
    }

    Cards cardsInHand {5};
};

TEST_F(EventsProcessorLeftMouseButtonUpEventTests,
       tryPutCardsBackFromHandOnLeftButtonUpEvent)
{
    ignoreLeftButtonUpOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonUpOnTableauPilesFromFirstTo(tableauPilesCount);
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorLeftMouseButtonUpEventTests,
       tryAddCardOnFoundationPileOnLeftButtonUpEvent)
{
    ignoreLeftButtonUpOnFoundationPilesFromFirstTo(foundationPilesCount - 2);
    acceptLeftButtonUpOnFoundationPile(beforeLastFoundationPileId, cardsInHandPosition);
    EXPECT_CALL(solitaireMock, tryAddCardOnFoundationPile(beforeLastFoundationPileId));
    acceptLeftButtonUpOnFoundationPile(lastFoundationPileId, cardsInHandPosition);
    tryAddCardOnFoundationPileAndClearHand(lastFoundationPileId);
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorLeftMouseButtonUpEventTests,
       tryAddCardsOnTableauPileOnLeftButtonUpEvent)
{
    ignoreLeftButtonUpOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonUpOnTableauPilesFromFirstTo(tableauPilesCount - 2);
    acceptLeftButtonUpOnTableauPile(beforeLastTableauPileId, cardsInHandPosition);
    EXPECT_CALL(solitaireMock, tryAddCardsOnTableauPile(beforeLastTableauPileId));
    acceptLeftButtonUpOnTableauPile(lastTableauPileId, cardsInHandPosition);
    tryAddCardsOnTableauPileAndClearHand(lastTableauPileId);
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, setButtonsHoveredAndMoveCardsInHandOnMoveEvent) {
    expectEvent(mouseMoveEvent);
    EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseMoveEvent.position)).WillOnce(Return(true));
    EXPECT_CALL(buttonMock, setHoveredState(true));
    EXPECT_CALL(contextMock, getUndoButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseMoveEvent.position)).WillOnce(Return(true));
    EXPECT_CALL(buttonMock, setHoveredState(true));
    expectMoveCardsInHand(mouseMoveEvent);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, setNewGameButtonNotHoveredOnMoveEvent) {
    expectEvent(mouseMoveEvent);
    EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseMoveEvent.position)).WillOnce(Return(false));
    EXPECT_CALL(buttonMock, setHoveredState(false));
    EXPECT_CALL(contextMock, getUndoButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, collidesWith(mouseMoveEvent.position)).WillOnce(Return(false));
    EXPECT_CALL(buttonMock, setHoveredState(false));
    expectMoveCardsInHand(mouseMoveEvent);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

}
