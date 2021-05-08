#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "colliders/FoundationPileColliderMock.h"
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
const Cards cards {5};

constexpr unsigned foundationPilesCount = 4;
constexpr unsigned tableauPilesCount = 7;
constexpr unsigned middleTableauPileCardIndex = 2;
const unsigned lastTableauPileCardIndex = cards.size() - 1;

const PileId lastFoundationPileId {foundationPilesCount - 1};
const PileId lastTableauPileId {tableauPilesCount - 1};

constexpr Position mousePosition {36, 12};
constexpr Position cardsInHandPosition {46, 26};
constexpr Position pilePosition {45, 93};

constexpr MouseLeftButtonDown mouseLeftButtonDownEvent {Position {24, 51}};
constexpr MouseMove mouseMoveEvent {Position {99, 13}};
}

class EventsProcessorTests: public Test {
public:
    InSequence seq;
    StrictMock<SolitaireMock> solitaireMock;
    StrictMock<FoundationPileColliderMock> foundationPileColliderMock;
    StrictMock<TableauPileColliderMock> tableauPileColliderMock;
    StrictMock<TableauPileMock> tableauPileMock;
    StrictMock<ContextMock> contextMock;
    mock_ptr<StrictMock<EventsSourceMock>> eventsSourceMock;

    EventsProcessor eventsProcessor {
        contextMock, eventsSourceMock.make_unique()};

    void expectEvent(const Event& event) {
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(event));
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

    void ignoreLeftButtonUpOnFoundationPilesFromFirstTo(unsigned pilesToIgnore) {
        for (PileId id {0}; id < pilesToIgnore; ++id) {
            EXPECT_CALL(contextMock, getFoundationPileCollider(id))
                .WillOnce(ReturnRef(foundationPileColliderMock));
            EXPECT_CALL(foundationPileColliderMock, collidesWithCardsInHand(_))
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
};

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingWhenNoEvents)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    expectEvent(NoEvents {});
    eventsProcessor.processEvents();
    EXPECT_FALSE(eventsProcessor.shouldQuit());
}

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingOnQuitEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    ignoreLeftButtonDownOnTableauPilesFromFirstTo(tableauPilesCount);
    expectEvent(Quit {});
    eventsProcessor.processEvents();
    EXPECT_TRUE(eventsProcessor.shouldQuit());
}

TEST_F(EventsProcessorTests,
       tryPullOutCardFromFoundationPileOnLeftButtonDownEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount - 1);
    acceptLeftButtonDownOnFoundationPile(lastFoundationPileId, mouseLeftButtonDownEvent);
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, tryPullOutCardFromFoundationPile(lastFoundationPileId));
    EXPECT_CALL(contextMock, setMousePosition(mouseLeftButtonDownEvent.position));
    EXPECT_CALL(foundationPileColliderMock, getPosition())
        .WillOnce(Return(pilePosition));
    EXPECT_CALL(contextMock, setCardsInHandPosition(pilePosition));
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests,
       tryUncoverTableauPileTopCardOnLeftButtonDownEventOnTopCard)
{
    expectEvent(mouseLeftButtonDownEvent);
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

TEST_F(EventsProcessorTests, tryPutCardsBackFromHandOnLeftButtonUpEvent) {
    expectEvent(MouseLeftButtonUp {});
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
    ignoreLeftButtonUpOnFoundationPilesFromFirstTo(foundationPilesCount);
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, tryAddCardOnFoundationPileOnLeftButtonUpEvent) {
    expectEvent(MouseLeftButtonUp {});
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
    ignoreLeftButtonUpOnFoundationPilesFromFirstTo(foundationPilesCount - 1);
    acceptLeftButtonUpOnFoundationPile(lastFoundationPileId, cardsInHandPosition);
    EXPECT_CALL(solitaireMock, tryAddCardOnFoundationPile(lastFoundationPileId));
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

TEST_F(EventsProcessorTests, moveCardsInHandOnMoveEvent) {
    expectEvent(mouseMoveEvent);
    EXPECT_CALL(contextMock, getMousePosition()).WillOnce(Return(mousePosition));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(cardsInHandPosition));
    EXPECT_CALL(contextMock, setMousePosition(mouseMoveEvent.position));
    const auto mouseMoveDelta = mouseMoveEvent.position - mousePosition;
    EXPECT_CALL(contextMock, setCardsInHandPosition(cardsInHandPosition + mouseMoveDelta));
    expectEvent(Quit {});
    eventsProcessor.processEvents();
}

}
