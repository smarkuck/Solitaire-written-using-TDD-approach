#include "Application.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "events/EventsProcessorMock.h"
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
    mock_ptr<SolitaireMock> solitaireMock;
    mock_ptr<EventsProcessorMock> eventsProcessorMock;
    mock_ptr<RendererMock> rendererMock;
    mock_ptr<FPSLimiterMock> fpsLimiterMock;

    Application application {solitaireMock.make_unique(),
                             eventsProcessorMock.make_unique(),
                             rendererMock.make_unique(),
                             fpsLimiterMock.make_unique()};

    void expectApplicationLoop() {
        EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
        EXPECT_CALL(*eventsProcessorMock, processEvents());
        EXPECT_CALL(*rendererMock, render());
        EXPECT_CALL(*fpsLimiterMock, sleepRestOfFrameTime());
    }
};

TEST_F(ApplicationTests, onRunStartNewGame) {
    InSequence seq;
    EXPECT_CALL(*solitaireMock, startNewGame());
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(true));
    application.run();
}

TEST_F(ApplicationTests, executeLoopUntilEventsProcessorIndicatesQuit) {
    InSequence seq;
    EXPECT_CALL(*solitaireMock, startNewGame());
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(false));
    expectApplicationLoop();
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(false));
    expectApplicationLoop();
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(true));
    application.run();
}

}
