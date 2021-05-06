#include "Application.h"
#include "ContextMock.h"
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
    void expectApplicationLoop() {
        EXPECT_CALL(*fpsLimiterMock, saveFrameStartTime());
        EXPECT_CALL(*eventsProcessorMock, processEvents());
        EXPECT_CALL(*rendererMock, render());
        EXPECT_CALL(*fpsLimiterMock, sleepRestOfFrameTime());
    }

    InSequence seq;
    SolitaireMock solitaireMock;
    mock_ptr<ContextMock> contextMock;
    mock_ptr<EventsProcessorMock> eventsProcessorMock;
    mock_ptr<RendererMock> rendererMock;
    mock_ptr<FPSLimiterMock> fpsLimiterMock;

    Application application {contextMock.make_unique(),
                             eventsProcessorMock.make_unique(),
                             rendererMock.make_unique(),
                             fpsLimiterMock.make_unique()};
};

TEST_F(ApplicationTests, onRunStartNewGame) {
    EXPECT_CALL(*contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, startNewGame());
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(true));
    application.run();
}

TEST_F(ApplicationTests, executeLoopUntilEventsProcessorIndicatesQuit) {
    EXPECT_CALL(*contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, startNewGame());
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(false));
    expectApplicationLoop();
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(false));
    expectApplicationLoop();
    EXPECT_CALL(*eventsProcessorMock, shouldQuit()).WillOnce(Return(true));
    application.run();
}

}
