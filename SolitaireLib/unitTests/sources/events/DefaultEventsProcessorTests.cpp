#include <array>

#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"
#include "graphics/TexturePosition.h"
#include "graphics/TextureSize.h"
#include "piles/PileId.h"

using namespace testing;
using namespace solitaire::graphics;
using namespace solitaire::piles;

namespace solitaire::events {

namespace {
constexpr unsigned foundationPilesCount = 4;

constexpr TextureSize cardSize {75, 104};

constexpr std::array<TexturePosition, foundationPilesCount> foundationPilesPositions {
    TexturePosition {283, 30},
    TexturePosition {372, 30},
    TexturePosition {461, 30},
    TexturePosition {550, 30}
};
}

class DefaultEventsProcessorTests: public Test {
public:
    MouseLeftButtonDown getClickEventOnePixelBeforeFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            pilePosition.x - 1,
            pilePosition.y + cardSize.height/2
        };
    }

    MouseLeftButtonDown getClickEventOnePixelAfterFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            pilePosition.x + cardSize.width,
            pilePosition.y + cardSize.height/2
        };
    }

    MouseLeftButtonDown getClickEventOnePixelAboveFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            pilePosition.x + cardSize.width/2,
            pilePosition.y - 1
        };
    }

    MouseLeftButtonDown getClickEventOnePixelBelowFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            pilePosition.x + cardSize.width/2,
            pilePosition.y + cardSize.height
        };
    }

    StrictMock<SolitaireMock> solitaireMock;
    StrictMock<ContextMock> contextMock;
    mock_ptr<StrictMock<EventsSourceMock>> eventsSourceMock;
    DefaultEventsProcessor eventsProcessor {
        solitaireMock, contextMock, eventsSourceMock.make_unique()};
};

TEST_F(DefaultEventsProcessorTests, stopProcessingWhenNoEvents) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(TexturePosition {0, 0}));
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(NoEvents {}));
    eventsProcessor.processEvents();
    EXPECT_FALSE(eventsProcessor.shouldQuit());
}

TEST_F(DefaultEventsProcessorTests, stopProcessingOnQuitEvent) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(TexturePosition {0, 0}));
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
    EXPECT_TRUE(eventsProcessor.shouldQuit());
}

TEST_F(DefaultEventsProcessorTests, ignoreLeftButtonDownEventOnPixelNextToFoundationPile) {
    InSequence seq;

    for (const auto& position: foundationPilesPositions) {
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(getClickEventOnePixelBeforeFoundationPile(position)));
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(getClickEventOnePixelAfterFoundationPile(position)));
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(getClickEventOnePixelAboveFoundationPile(position)));
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(getClickEventOnePixelBelowFoundationPile(position)));
    }

    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
}

TEST_F(DefaultEventsProcessorTests, tryPullOutCardFromFoundationPileOnLeftButtonDownEvent) {
    InSequence seq;
    for (PileId id {0}; id < foundationPilesCount; ++id) {
        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(MouseLeftButtonDown {
                foundationPilesPositions[id].x,
                foundationPilesPositions[id].y}));
        EXPECT_CALL(solitaireMock, tryPullOutCardFromFoundationPile(id));
        EXPECT_CALL(contextMock, setMousePosition(foundationPilesPositions[id]));
        EXPECT_CALL(contextMock, setCardsInHandPosition(foundationPilesPositions[id]));

        EXPECT_CALL(*eventsSourceMock, getEvent())
            .WillOnce(Return(MouseLeftButtonDown {
                foundationPilesPositions[id].x + cardSize.width - 1,
                foundationPilesPositions[id].y + cardSize.height - 1}));
        EXPECT_CALL(solitaireMock, tryPullOutCardFromFoundationPile(id));
        EXPECT_CALL(contextMock, setMousePosition(TexturePosition {
            foundationPilesPositions[id].x + cardSize.width - 1,
            foundationPilesPositions[id].y + cardSize.height - 1
        }));
        EXPECT_CALL(contextMock, setCardsInHandPosition(foundationPilesPositions[id]));
    }
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
}

TEST_F(DefaultEventsProcessorTests, moveCardsInHandOnMoveEvent) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseMove {10, 20}));
    EXPECT_CALL(contextMock, getMousePosition()).WillOnce(Return(TexturePosition {50, 10}));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(TexturePosition {60, 15}));
    EXPECT_CALL(contextMock, setMousePosition(TexturePosition {10, 20}));
    EXPECT_CALL(contextMock, setCardsInHandPosition(TexturePosition {20, 25}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
}

TEST_F(DefaultEventsProcessorTests, tryPutCardsBackFromHandOnLeftButtonUpEventOnEmptySpace) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(TexturePosition {0, 0}));
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
}

TEST_F(DefaultEventsProcessorTests, tryAddCardOnLeftButtonUpEventOnFoundationPile) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(contextMock, getCardsInHandPosition()).WillOnce(Return(TexturePosition {
            foundationPilesPositions[0].x - cardSize.width + 1, foundationPilesPositions[0].y}));
    EXPECT_CALL(solitaireMock, tryAddCardOnFoundationPile(PileId {0}));
    EXPECT_CALL(solitaireMock, tryPutCardsBackFromHand());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
}

}
