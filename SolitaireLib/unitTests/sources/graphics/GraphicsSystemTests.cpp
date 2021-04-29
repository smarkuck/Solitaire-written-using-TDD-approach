#include "mock_ptr.h"
#include "gmock/gmock.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/SDLDeleterMock.h"
#include "graphics/SDLWrapperMock.h"
#include "graphics/TextureId.h"

using namespace testing;

namespace solitaire::graphics {

namespace {
const std::string title {"Solitaire"};
const std::string texturePath {"texture.bmp"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {840};
constexpr Uint32 mapRGBResult {10};

SDL_Window* const windowPtr {reinterpret_cast<SDL_Window*>(1)};
SDL_Renderer* const rendererPtr {reinterpret_cast<SDL_Renderer*>(2)};
SDL_Texture* const texturePtr {reinterpret_cast<SDL_Texture*>(3)};
SDL_Texture* const texturePtr2 {reinterpret_cast<SDL_Texture*>(4)};
SDL_Surface* const surfacePtr {reinterpret_cast<SDL_Surface*>(5)};
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

    void expectThrowIfSDLCreateRendererFailedDuringWindowCreation() {
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

    void expectQuitSystem() {
        EXPECT_CALL(*SDLPtrDeleterMock, rendererDeleter(rendererPtr));
        EXPECT_CALL(*SDLPtrDeleterMock, windowDeleter(windowPtr));
        EXPECT_CALL(*SDLMock, quit());
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
    expectThrowIfSDLCreateRendererFailedDuringWindowCreation();
}

TEST_F(GraphicsSystemTests, createWindowSuccessfully) {
    InSequence seq;
    expectCreateWindow();
    expectQuitSystem();
}

TEST_F(GraphicsSystemTests, createWindowSuccessfullyOnSecondTry) {
    InSequence seq;
    expectThrowIfSDLCreateRendererFailedDuringWindowCreation();
    expectCreateWindow();
    expectQuitSystem();
}

TEST_F(GraphicsSystemTests, throwOnCreateWindowAfterSuccessfulCreation) {
    InSequence seq;
    expectCreateWindow();
    EXPECT_THROW(system.createWindow(title, windowWidth, windowHeight), std::runtime_error);
    expectQuitSystem();
}

TEST_F(GraphicsSystemTests, throwOnLoadTextureIfWindowNotCreated) {
    InSequence seq;
    expectThrowIfSDLCreateRendererFailedDuringWindowCreation();
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
}

class GraphicsSystemWithCreatedWindowTests: public GraphicsSystemTests {
public:
    GraphicsSystemWithCreatedWindowTests() {
        expectCreateWindow();
    }

    SDLPtr<SDL_Texture> makeSDLTexture(SDL_Texture* ptr) {
        return SDLPtr<SDL_Texture> {ptr, SDLDeleter{SDLPtrDeleterMock}};
    }

    SDLPtr<SDL_Surface> makeSDLSurface(SDL_Surface* ptr) {
        return SDLPtr<SDL_Surface> {ptr, SDLDeleter{SDLPtrDeleterMock}};
    }

    void expectCreateTexture(SDL_Texture* texturePtr, TextureId textureId) {
        EXPECT_CALL(*SDLMock, loadBMP(texturePath))
            .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
        EXPECT_CALL(*SDLMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
            .WillOnce(Return(mapRGBResult));
        EXPECT_CALL(*SDLMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
            .WillOnce(Return(0));
        EXPECT_CALL(*SDLMock, createTextureFromSurface(Pointer(rendererPtr), Pointer(surfacePtr)))
            .WillOnce(Return(ByMove(makeSDLTexture(texturePtr))));
        EXPECT_CALL(*SDLPtrDeleterMock, surfaceDeleter(surfacePtr));
        EXPECT_EQ(system.loadTexture(texturePath), textureId);
    }

};

TEST_F(GraphicsSystemWithCreatedWindowTests,
    throwIfSDLLoadBMPFailedDuringLoadTexture)
{
    InSequence seq;
    EXPECT_CALL(*SDLMock, loadBMP(texturePath)).WillOnce(ReturnNull());
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(GraphicsSystemWithCreatedWindowTests,
    throwIfSDLSetColorKeyFailedDuringLoadTexture)
{
    InSequence seq;
    EXPECT_CALL(*SDLMock, loadBMP(texturePath))
        .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
    EXPECT_CALL(*SDLMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
        .WillOnce(Return(mapRGBResult));
    EXPECT_CALL(*SDLMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
        .WillOnce(Return(-1));
    EXPECT_CALL(*SDLPtrDeleterMock, surfaceDeleter(surfacePtr));
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(GraphicsSystemWithCreatedWindowTests,
    throwIfSDLCreateTextureFromSurfaceFailedDuringLoadTexture)
{
    InSequence seq;
    EXPECT_CALL(*SDLMock, loadBMP(texturePath))
        .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
    EXPECT_CALL(*SDLMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
        .WillOnce(Return(mapRGBResult));
    EXPECT_CALL(*SDLMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
        .WillOnce(Return(0));
    EXPECT_CALL(*SDLMock, createTextureFromSurface(Pointer(rendererPtr), Pointer(surfacePtr)))
        .WillOnce(ReturnNull());
    EXPECT_CALL(*SDLPtrDeleterMock, surfaceDeleter(surfacePtr));
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(GraphicsSystemWithCreatedWindowTests, loadTextureSuccessfully)
{
    InSequence seq;
    expectCreateTexture(texturePtr, TextureId{0});
    EXPECT_CALL(*SDLPtrDeleterMock, textureDeleter(texturePtr));
    expectQuitSystem();
}

TEST_F(GraphicsSystemWithCreatedWindowTests, loadSecondTexture)
{
    InSequence seq;
    expectCreateTexture(texturePtr, TextureId{0});
    expectCreateTexture(texturePtr2, TextureId{1});
    EXPECT_CALL(*SDLPtrDeleterMock, textureDeleter(texturePtr));
    EXPECT_CALL(*SDLPtrDeleterMock, textureDeleter(texturePtr2));
    expectQuitSystem();
}

}
