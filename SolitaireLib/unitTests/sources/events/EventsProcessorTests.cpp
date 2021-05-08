#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "colliders/FoundationPileColliderMock.h"
#include "events/EventsDefinitions.h"
#include "events/EventsProcessor.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace solitaire::colliders;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::events {

namespace {
constexpr unsigned foundationPilesCount = 4;
const PileId lastFoundationPileId {3};

constexpr Position mousePosition {36, 12};
constexpr Position cardsInHandPosition {46, 26};
constexpr Position foundationPilePosition {45, 93};

constexpr MouseLeftButtonDown mouseLeftButtonDownEvent {Position {24, 51}};
constexpr MouseMove mouseMoveEvent {Position {99, 13}};
}

class EventsProcessorTests: public Test {
public:
    InSequence seq;
    StrictMock<SolitaireMock> solitaireMock;
    StrictMock<FoundationPileColliderMock> foundationPileColliderMock;
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
};

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingWhenNoEvents)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
    expectEvent(NoEvents {});
    eventsProcessor.processEvents();
    EXPECT_FALSE(eventsProcessor.shouldQuit());
}

TEST_F(EventsProcessorTests,
       ignoreLeftButtonDownEventAndStopProcessingOnQuitEvent)
{
    expectEvent(mouseLeftButtonDownEvent);
    ignoreLeftButtonDownOnFoundationPilesFromFirstTo(foundationPilesCount);
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
        .WillOnce(Return(foundationPilePosition));
    EXPECT_CALL(contextMock, setCardsInHandPosition(foundationPilePosition));
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
