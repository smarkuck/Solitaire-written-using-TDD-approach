#include "Application.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSourceMock.h"
#include "gmock/gmock.h"
#include "graphics/RendererMock.h"

using namespace testing;
using namespace solitaire::events;
using namespace solitaire::graphics;

namespace solitaire {

class ApplicationTests: public Test {
public:
    mock_ptr<StrictMock<SolitaireMock>> solitaireMock;
    mock_ptr<StrictMock<EventsSourceMock>> eventsSourceMock;
    mock_ptr<StrictMock<RendererMock>> rendererMock;

    Application application {solitaireMock.make_unique(),
                             eventsSourceMock.make_unique(),
                             rendererMock.make_unique()};
};

TEST_F(ApplicationTests, quitApplicationOnQuitEvent) {
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    application.run();
}

TEST_F(ApplicationTests, onMouseLeftButtonDownEventDoNothingIfClickedEmptySpace) {
    InSequence seq;
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(MouseLeftButtonDown {0, 0}));
    EXPECT_CALL(*rendererMock, render());
    EXPECT_CALL(*eventsSourceMock, getEvent()).WillOnce(Return(Quit {}));
    application.run();
}

}
