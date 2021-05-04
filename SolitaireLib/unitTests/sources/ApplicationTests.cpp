#include "Application.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"
#include "graphics/RendererMock.h"
#include "time/FPSLimiterMock.h"

using namespace testing;
using namespace solitaire::events;
using namespace solitaire::graphics;
using namespace solitaire::time;

namespace solitaire {

class ApplicationTests: public Test {
public:
    mock_ptr<StrictMock<SolitaireMock>> solitaireMock;
    mock_ptr<StrictMock<EventsSourceMock>> eventsSourceMock;
    mock_ptr<StrictMock<RendererMock>> rendererMock;
    mock_ptr<StrictMock<FPSLimiterMock>> fpsLimiterMock;

    Application application {solitaireMock.make_unique(),
                             eventsSourceMock.make_unique(),
                             rendererMock.make_unique(),
                             fpsLimiterMock.make_unique()};
};

TEST_F(ApplicationTests, onRunStartNewGameAndQuitOnQuitEvent) {
    InSequence seq;
    EXPECT_CALL(*solitaireMock, startNewGame());
    EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    application.run();
}

TEST_F(ApplicationTests, quitImmediatelyOnQuitEvent) {
    InSequence seq;
    EXPECT_CALL(*solitaireMock, startNewGame());
    EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    application.run();
}

TEST_F(ApplicationTests, processAllEventsBeforeRender) {
    InSequence seq;
    EXPECT_CALL(*solitaireMock, startNewGame());
    EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonUp {0, 0}));
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(NoEvents {}));
    EXPECT_CALL(*rendererMock, render());
    EXPECT_CALL(*fpsLimiterMock, sleepRestOfFrameTime());
    EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    application.run();
}

}
