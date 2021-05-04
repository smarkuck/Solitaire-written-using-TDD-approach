#include "mock_ptr.h"
#include "events/EventsDefinitions.h"
#include "events/SDLEventsSource.h"
#include "gmock/gmock.h"
#include "SDL/WrapperMock.h"

using namespace testing;
using namespace solitaire::SDL;

namespace solitaire::events {

namespace {
constexpr int x {100};
constexpr int y {120};
}

class SDLEventsSourceTests: public Test {
public:
    mock_ptr<WrapperMock> sdlMock;
    SDLEventsSource eventsSource {sdlMock.make_unique()};
    SDL_Event event;
};

TEST_F(SDLEventsSourceTests, returnNoEventsIfSDLPollEventReturnsZero) {
    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Return(0));
    EXPECT_EQ(std::get<NoEvents>(eventsSource.getEvent()), NoEvents {});
}

TEST_F(SDLEventsSourceTests, ignoreEventsIfSDLPollEventReturnsNotSupportedEvents) {
    InSequence seq;
    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_KEYDOWN;
        return 1;
    }));

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEWHEEL;
        return 1;
    }));

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Return(0));
    EXPECT_EQ(std::get<NoEvents>(eventsSource.getEvent()), NoEvents {});
}

TEST_F(SDLEventsSourceTests, ignoreEventIfSDLPollEventReturnsNotLeftMouseButtonEvent) {
    InSequence seq;
    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEBUTTONDOWN;
        event.button.button = SDL_BUTTON_RIGHT;
        return 1;
    }));

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEBUTTONUP;
        event.button.button = SDL_BUTTON_RIGHT;
        return 1;
    }));

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Return(0));
    EXPECT_EQ(std::get<NoEvents>(eventsSource.getEvent()), NoEvents {});
}

TEST_F(SDLEventsSourceTests, returnMouseLeftButtonDownEvent) {
    InSequence seq;

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEBUTTONDOWN;
        event.button.button = SDL_BUTTON_LEFT;
        event.button.x = x;
        event.button.y = y;
        return 1;
    }));

    MouseLeftButtonDown mouseLeftButtonDown {x, y};
    EXPECT_EQ(std::get<MouseLeftButtonDown>(eventsSource.getEvent()), mouseLeftButtonDown);
}

TEST_F(SDLEventsSourceTests, returnMouseLeftButtonUpEvent) {
    InSequence seq;

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEBUTTONUP;
        event.button.button = SDL_BUTTON_LEFT;
        event.button.x = x;
        event.button.y = y;
        return 1;
    }));

    MouseLeftButtonUp mouseLeftButtonUp {x, y};
    EXPECT_EQ(std::get<MouseLeftButtonUp>(eventsSource.getEvent()), mouseLeftButtonUp);
}

TEST_F(SDLEventsSourceTests, returnMouseMoveEvent) {
    InSequence seq;

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_MOUSEMOTION;
        event.motion.x = x;
        event.motion.y = y;
        return 1;
    }));

    MouseMove mouseMove {x, y};
    EXPECT_EQ(std::get<MouseMove>(eventsSource.getEvent()), mouseMove);
}

TEST_F(SDLEventsSourceTests, returnQuitEvent) {
    InSequence seq;

    EXPECT_CALL(*sdlMock, pollEvent(_)).WillOnce(Invoke([](auto& event) {
        event.type = SDL_QUIT;
        return 1;
    }));

    EXPECT_EQ(std::get<Quit>(eventsSource.getEvent()), Quit {});
}

}
