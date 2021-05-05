#include <array>

#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"
#include "graphics/TexturePosition.h"
#include "graphics/TextureSize.h"

using namespace testing;
using namespace solitaire::graphics;

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
    SolitaireMock solitaireMock;
    mock_ptr<EventsSourceMock> eventsSourceMock;
    DefaultEventsProcessor eventsProcessor {solitaireMock, eventsSourceMock.make_unique()};

    MouseLeftButtonDown getClickEventOnePixelBeforeFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            static_cast<int>(pilePosition.x - 1),
            static_cast<int>(pilePosition.y + cardSize.height/2)
        };
    }

    MouseLeftButtonDown getClickEventOnePixelAfterFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            static_cast<int>(pilePosition.x + cardSize.width),
            static_cast<int>(pilePosition.y + cardSize.height/2)
        };
    }

    MouseLeftButtonDown getClickEventOnePixelAboveFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            static_cast<int>(pilePosition.x + cardSize.width/2),
            static_cast<int>(pilePosition.y - 1)
        };
    }

    MouseLeftButtonDown getClickEventOnePixelBelowFoundationPile(
        const TexturePosition& pilePosition)
    {
        return MouseLeftButtonDown {
            static_cast<int>(pilePosition.x + cardSize.width/2),
            static_cast<int>(pilePosition.y + cardSize.height)
        };
    }
};

TEST_F(DefaultEventsProcessorTests, stopProcessingWhenNoEvents) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(NoEvents {}));
    eventsProcessor.processEvents();
    EXPECT_FALSE(eventsProcessor.shouldQuit());
}

TEST_F(DefaultEventsProcessorTests, stopProcessingOnQuitEvent) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    eventsProcessor.processEvents();
    EXPECT_TRUE(eventsProcessor.shouldQuit());
}

TEST_F(DefaultEventsProcessorTests, ignoreLeftClickOnPixelNextToFoundationPile) {
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

}
