#include "mock_ptr.h"
#include "gmock/gmock.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/SDLDeleterMock.h"
#include "graphics/SDLWrapperMock.h"

using namespace testing;

namespace solitaire::graphics {

namespace {
const std::string title {"Solitaire"};
constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {840};

SDL_Window* const windowPtr {reinterpret_cast<SDL_Window*>(1)};
SDL_Renderer* const rendererPtr {reinterpret_cast<SDL_Renderer*>(2)};
}

class GraphicsSystemTests: public Test {
public:
    std::shared_ptr<StrictMock<SDLDeleterMock>> SDLPtrDeleterMock {
        std::make_shared<StrictMock<SDLDeleterMock>>()};

    mock_ptr<StrictMock<SDLWrapperMock>> SDLMock;
    GraphicsSystem system {SDLMock.make_unique()};

    auto& expectSDLCreateWindow(
        const std::string& title, unsigned width, unsigned height)
    {
        return EXPECT_CALL(*SDLMock, createWindow(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN)
        );
    }

    auto& expectSDLCreateRenderer(SDL_Window* ptr) {
        return EXPECT_CALL(*SDLMock, createRenderer(
            Pointer(ptr), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
        );
    }

    SDLPtr<SDL_Window> makeSDLWindow(SDL_Window* ptr) {
        return SDLPtr<SDL_Window> {ptr, SDLDeleter{SDLPtrDeleterMock}};
    }

    SDLPtr<SDL_Renderer> makeSDLRenderer(SDL_Renderer* ptr) {
        return SDLPtr<SDL_Renderer> {ptr, SDLDeleter{SDLPtrDeleterMock}};
    }

    void throwIfSDLCreateRendererFailedDuringWindowCreationTest() {
        EXPECT_CALL(*SDLMock, init(SDL_INIT_VIDEO)).WillOnce(Return(0));
        expectSDLCreateWindow(title, windowWidth, windowHeight)
            .WillOnce(Return(ByMove(makeSDLWindow(windowPtr))));
        expectSDLCreateRenderer(windowPtr).WillOnce(ReturnNull());
        EXPECT_CALL(*SDLPtrDeleterMock, windowDeleter(windowPtr));
        EXPECT_CALL(*SDLMock, quit());
        EXPECT_THROW(system.createWindow(title, windowWidth, windowHeight), std::runtime_error);
    }

    void expectCreateWindow() {
        EXPECT_CALL(*SDLMock, init(SDL_INIT_VIDEO)).WillOnce(Return(0));
        expectSDLCreateWindow(title, windowWidth, windowHeight)
            .WillOnce(Return(ByMove(makeSDLWindow(windowPtr))));
        expectSDLCreateRenderer(windowPtr)
            .WillOnce(Return(ByMove(makeSDLRenderer(rendererPtr))));
        system.createWindow(title, windowWidth, windowHeight);
    }

    void expectFullQuitSystem() {
        EXPECT_CALL(*SDLPtrDeleterMock, rendererDeleter(rendererPtr));
        EXPECT_CALL(*SDLPtrDeleterMock, windowDeleter(windowPtr));
        EXPECT_CALL(*SDLMock, quit());
    }

    void createWindowSuccessfullyTest() {
        expectCreateWindow();
        expectFullQuitSystem();
    }
};

TEST_F(GraphicsSystemTests, doNothingWithSDLWhenWindowNotCreated) {
}

TEST_F(GraphicsSystemTests, throwIfSDLInitFailedDuringWindowCreation) {
    InSequence seq;
    EXPECT_CALL(*SDLMock, init(SDL_INIT_VIDEO)).WillOnce(Return(-1));
    EXPECT_THROW(system.createWindow(title, windowWidth, windowHeight), std::runtime_error);
}

TEST_F(GraphicsSystemTests, throwIfSDLCreateWindowFailedDuringWindowCreation) {
    InSequence seq;
    EXPECT_CALL(*SDLMock, init(SDL_INIT_VIDEO)).WillOnce(Return(0));
    expectSDLCreateWindow(title, windowWidth, windowHeight).WillOnce(ReturnNull());
    EXPECT_CALL(*SDLMock, quit());
    EXPECT_THROW(system.createWindow(title, windowWidth, windowHeight), std::runtime_error);
}

TEST_F(GraphicsSystemTests, throwIfSDLCreateRendererFailedDuringWindowCreation) {
    InSequence seq;
    throwIfSDLCreateRendererFailedDuringWindowCreationTest();
}

TEST_F(GraphicsSystemTests, createWindowSuccessfully) {
    InSequence seq;
    createWindowSuccessfullyTest();
}

TEST_F(GraphicsSystemTests, createWindowSuccessfullyOnSecondTry) {
    InSequence seq;
    throwIfSDLCreateRendererFailedDuringWindowCreationTest();
    createWindowSuccessfullyTest();
}

TEST_F(GraphicsSystemTests, throwOnCreateWindowAfterSuccessfulCreation) {
    InSequence seq;
    expectCreateWindow();
    EXPECT_THROW(system.createWindow(title, windowWidth, windowHeight), std::runtime_error);
    expectFullQuitSystem();
}

}
