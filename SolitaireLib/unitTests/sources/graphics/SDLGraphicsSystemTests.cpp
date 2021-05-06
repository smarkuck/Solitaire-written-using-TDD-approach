#include "mock_ptr.h"
#include "gmock/gmock.h"
#include "geometry/Area.h"
#include "graphics/SDLGraphicsSystem.h"
#include "graphics/TextureId.h"
#include "SDL/PtrDeleterMock.h"
#include "SDL/WrapperMock.h"

using namespace testing;
using namespace solitaire::geometry;
using namespace solitaire::SDL;

namespace solitaire::graphics {

namespace {
const std::string title {"Solitaire"};
const std::string texturePath {"texture.bmp"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {840};
constexpr Uint32 mapRGBResult {10};
constexpr uint8_t alpha {70};
constexpr int success {0};
constexpr int failure {-1};

const TextureId textureId0 {0};
const TextureId textureId1 {1};

constexpr Position texturePosition {13, 17};
constexpr Area textureArea {Position {10, 15}, Size {20, 30}};

constexpr SDL_Rect srcRect {
    textureArea.position.x, textureArea.position.y,
    textureArea.size.width, textureArea.size.height
};

constexpr SDL_Rect dstRect {
    texturePosition.x, texturePosition.y,
    textureArea.size.width, textureArea.size.height
};

SDL_Window* const windowPtr {reinterpret_cast<SDL_Window*>(1)};
SDL_Renderer* const rendererPtr {reinterpret_cast<SDL_Renderer*>(2)};
SDL_Texture* const texture0Ptr {reinterpret_cast<SDL_Texture*>(3)};
SDL_Texture* const texture1Ptr {reinterpret_cast<SDL_Texture*>(4)};
SDL_Surface* const surfacePtr {reinterpret_cast<SDL_Surface*>(5)};

MATCHER_P(IsOptionalEq, rect, "") {
    if (not arg)
        return false;

    auto& value = arg.value();
    return value.x == rect.x and value.y == rect.y and
           value.w == rect.w and value.h == rect.h;
}
}

class SDLGraphicsSystemTests: public Test {
public:
    std::shared_ptr<StrictMock<PtrDeleterMock>> ptrDeleterMock {
        std::make_shared<StrictMock<PtrDeleterMock>>()};

    mock_ptr<StrictMock<WrapperMock>> sdlMock;
    SDLGraphicsSystem system {sdlMock.make_unique()};

    auto& expectSDLCreateWindow(
        const std::string& title, unsigned width, unsigned height)
    {
        return EXPECT_CALL(*sdlMock, createWindow(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN)
        );
    }

    auto& expectSDLCreateRenderer(SDL_Window* ptr) {
        return EXPECT_CALL(*sdlMock, createRenderer(
            Pointer(ptr), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
        );
    }

    UniquePtr<SDL_Window> makeSDLWindow(SDL_Window* ptr) {
        return UniquePtr<SDL_Window> {ptr, PtrDeleter {ptrDeleterMock}};
    }

    UniquePtr<SDL_Renderer> makeSDLRenderer(SDL_Renderer* ptr) {
        return UniquePtr<SDL_Renderer> {ptr, PtrDeleter {ptrDeleterMock}};
    }

    void expectThrowIfSDLCreateRendererFailedDuringWindowCreation() {
        EXPECT_CALL(*sdlMock, init(SDL_INIT_VIDEO)).WillOnce(Return(success));
        expectSDLCreateWindow(title, windowWidth, windowHeight)
            .WillOnce(Return(ByMove(makeSDLWindow(windowPtr))));
        expectSDLCreateRenderer(windowPtr).WillOnce(ReturnNull());
        EXPECT_CALL(*ptrDeleterMock, windowDeleter(windowPtr));
        EXPECT_CALL(*sdlMock, quit());

        EXPECT_THROW(
            system.createWindow(title, windowWidth, windowHeight),
            std::runtime_error
        );
    }

    void expectCreateWindow() {
        EXPECT_CALL(*sdlMock, init(SDL_INIT_VIDEO)).WillOnce(Return(success));
        expectSDLCreateWindow(title, windowWidth, windowHeight)
            .WillOnce(Return(ByMove(makeSDLWindow(windowPtr))));
        expectSDLCreateRenderer(windowPtr)
            .WillOnce(Return(ByMove(makeSDLRenderer(rendererPtr))));
        system.createWindow(title, windowWidth, windowHeight);
    }

    void expectQuitSystem() {
        EXPECT_CALL(*ptrDeleterMock, rendererDeleter(rendererPtr));
        EXPECT_CALL(*ptrDeleterMock, windowDeleter(windowPtr));
        EXPECT_CALL(*sdlMock, quit());
    }

    InSequence seq;
};

TEST_F(SDLGraphicsSystemTests, doNothingWithSDLWhenWindowNotCreated) {}

TEST_F(SDLGraphicsSystemTests, throwIfSDLInitFailedDuringWindowCreation) {
    EXPECT_CALL(*sdlMock, init(SDL_INIT_VIDEO)).WillOnce(Return(failure));

    EXPECT_THROW(
        system.createWindow(title, windowWidth, windowHeight),
        std::runtime_error
    );
}

TEST_F(SDLGraphicsSystemTests, throwIfSDLCreateWindowFailedDuringWindowCreation) {
    EXPECT_CALL(*sdlMock, init(SDL_INIT_VIDEO)).WillOnce(Return(success));
    expectSDLCreateWindow(title, windowWidth, windowHeight).WillOnce(ReturnNull());
    EXPECT_CALL(*sdlMock, quit());

    EXPECT_THROW(
        system.createWindow(title, windowWidth, windowHeight),
        std::runtime_error
    );
}

TEST_F(SDLGraphicsSystemTests,
       throwIfSDLCreateRendererFailedDuringWindowCreation)
{
    expectThrowIfSDLCreateRendererFailedDuringWindowCreation();
}

TEST_F(SDLGraphicsSystemTests, createWindowSuccessfully) {
    expectCreateWindow();
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemTests, throwOnCreateWindowAfterSuccessfulCreation) {
    expectCreateWindow();

    EXPECT_THROW(
        system.createWindow(title, windowWidth, windowHeight),
        std::runtime_error
    );

    expectQuitSystem();
}

class SDLGraphicsSystemAfterWindowCreationFailedTests:
    public SDLGraphicsSystemTests
{
public:
    SDLGraphicsSystemAfterWindowCreationFailedTests() {
        expectThrowIfSDLCreateRendererFailedDuringWindowCreation();
    }
};

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       createWindowSuccessfullyOnSecondTry)
{
    expectCreateWindow();
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       throwOnLoadTextureIfWindowNotCreated)
{
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
}

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       throwOnSetTextureAlphaIfWindowNotCreated)
{
    EXPECT_THROW(system.setTextureAlpha(textureId0, alpha), std::runtime_error);
}

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       throwOnRenderTextureIfWindowNotCreated)
{
    EXPECT_THROW(
        system.renderTexture(textureId0, texturePosition, textureArea),
        std::runtime_error
    );
}

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       throwOnRenderTextureInFullWindowIfWindowNotCreated)
{
    EXPECT_THROW(system.renderTextureInFullWindow(textureId0), std::runtime_error);
}

TEST_F(SDLGraphicsSystemAfterWindowCreationFailedTests,
       throwOnRenderFrameIfWindowNotCreated)
{
    EXPECT_THROW(system.renderFrame(), std::runtime_error);
}

class SDLGraphicsSystemWithCreatedWindowTests: public SDLGraphicsSystemTests {
public:
    SDLGraphicsSystemWithCreatedWindowTests() {
        expectCreateWindow();
    }

    UniquePtr<SDL_Texture> makeSDLTexture(SDL_Texture* ptr) {
        return UniquePtr<SDL_Texture> {ptr, PtrDeleter {ptrDeleterMock}};
    }

    UniquePtr<SDL_Surface> makeSDLSurface(SDL_Surface* ptr) {
        return UniquePtr<SDL_Surface> {ptr, PtrDeleter {ptrDeleterMock}};
    }

    void expectCreateTexture(SDL_Texture* texturePtr, TextureId textureId) {
        EXPECT_CALL(*sdlMock, loadBMP(texturePath))
            .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
        EXPECT_CALL(*sdlMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
            .WillOnce(Return(mapRGBResult));
        EXPECT_CALL(*sdlMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
            .WillOnce(Return(success));
        EXPECT_CALL(*sdlMock,
            createTextureFromSurface(Pointer(rendererPtr), Pointer(surfacePtr))
        ).WillOnce(Return(ByMove(makeSDLTexture(texturePtr))));
        EXPECT_CALL(*ptrDeleterMock, surfaceDeleter(surfacePtr));
        EXPECT_EQ(system.loadTexture(texturePath), textureId);
    }
};

TEST_F(SDLGraphicsSystemWithCreatedWindowTests,
       throwIfSDLLoadBMPFailedDuringLoadTexture)
{
    EXPECT_CALL(*sdlMock, loadBMP(texturePath)).WillOnce(ReturnNull());
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemWithCreatedWindowTests,
       throwIfSDLSetColorKeyFailedDuringLoadTexture)
{
    EXPECT_CALL(*sdlMock, loadBMP(texturePath))
        .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
    EXPECT_CALL(*sdlMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
        .WillOnce(Return(mapRGBResult));
    EXPECT_CALL(*sdlMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
        .WillOnce(Return(failure));
    EXPECT_CALL(*ptrDeleterMock, surfaceDeleter(surfacePtr));
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemWithCreatedWindowTests,
       throwIfSDLCreateTextureFromSurfaceFailedDuringLoadTexture)
{
    EXPECT_CALL(*sdlMock, loadBMP(texturePath))
        .WillOnce(Return(ByMove(makeSDLSurface(surfacePtr))));
    EXPECT_CALL(*sdlMock, mapRGB(Pointer(surfacePtr), 0, 255, 255))
        .WillOnce(Return(mapRGBResult));
    EXPECT_CALL(*sdlMock, setColorKey(Pointer(surfacePtr), SDL_TRUE, mapRGBResult))
        .WillOnce(Return(success));
    EXPECT_CALL(*sdlMock,
        createTextureFromSurface(Pointer(rendererPtr), Pointer(surfacePtr))
    ).WillOnce(ReturnNull());
    EXPECT_CALL(*ptrDeleterMock, surfaceDeleter(surfacePtr));
    EXPECT_THROW(system.loadTexture(texturePath), std::runtime_error);
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemWithCreatedWindowTests, loadTextureSuccessfully) {
    expectCreateTexture(texture0Ptr, textureId0);
    EXPECT_CALL(*ptrDeleterMock, textureDeleter(texture0Ptr));
    expectQuitSystem();
}

class SDLGraphicsSystemWithLoadedTexture:
    public SDLGraphicsSystemWithCreatedWindowTests
{
public:
    SDLGraphicsSystemWithLoadedTexture() {
        expectCreateTexture(texture0Ptr, textureId0);
    }

    void expectQuitSystemWithLoadedTexture() {
        EXPECT_CALL(*ptrDeleterMock, textureDeleter(texture0Ptr));
        expectQuitSystem();
    }
};

TEST_F(SDLGraphicsSystemWithLoadedTexture, loadSecondTexture) {
    expectCreateTexture(texture1Ptr, textureId1);
    EXPECT_CALL(*ptrDeleterMock, textureDeleter(texture0Ptr));
    EXPECT_CALL(*ptrDeleterMock, textureDeleter(texture1Ptr));
    expectQuitSystem();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfUsedWrongTextureIdDuringSetTextureAlpha)
{
    EXPECT_THROW(system.setTextureAlpha(textureId1, alpha), std::runtime_error);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfSDLSetTextureAlphaModFailedDuringSetTextureAlpha)
{
    EXPECT_CALL(*sdlMock, setTextureAlphaMod(Pointer(texture0Ptr), alpha))
        .WillOnce(Return(failure));
    EXPECT_THROW(system.setTextureAlpha(textureId0, alpha), std::runtime_error);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture, setTextureAlphaSuccessfully) {
    EXPECT_CALL(*sdlMock, setTextureAlphaMod(Pointer(texture0Ptr), alpha))
        .WillOnce(Return(success));
    system.setTextureAlpha(textureId0, alpha);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfUsedWrongTextureIdDuringTextureRendering)
{
    EXPECT_THROW(
        system.renderTexture(textureId1, texturePosition, textureArea),
        std::runtime_error
    );

    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfSDLRenderCopyFailedDuringTextureRendering)
{
    EXPECT_CALL(*sdlMock,
        renderCopy(Pointer(rendererPtr), Pointer(texture0Ptr),
                   IsOptionalEq(srcRect), IsOptionalEq(dstRect))
    ).WillOnce(Return(failure));

    EXPECT_THROW(
        system.renderTexture(textureId0, texturePosition, textureArea),
        std::runtime_error
    );

    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture, renderTextureSuccessfully) {
    EXPECT_CALL(*sdlMock,
        renderCopy(Pointer(rendererPtr), Pointer(texture0Ptr),
                   IsOptionalEq(srcRect), IsOptionalEq(dstRect))
    ).WillOnce(Return(success));

    system.renderTexture(textureId0, texturePosition, textureArea);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfUsedWrongTextureIdDuringTextureRenderingInFullWindow)
{
    EXPECT_THROW(system.renderTextureInFullWindow(textureId1), std::runtime_error);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfSDLRenderCopyFailedDuringTextureRenderingInFullWindow)
{
    EXPECT_CALL(*sdlMock,
        renderCopy(Pointer(rendererPtr), Pointer(texture0Ptr),
                   Eq(std::nullopt), Eq(std::nullopt))
    ).WillOnce(Return(failure));

    EXPECT_THROW(system.renderTextureInFullWindow(textureId0), std::runtime_error);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       renderTextureInFullWindowSuccessfully)
{
    EXPECT_CALL(*sdlMock,
        renderCopy(Pointer(rendererPtr), Pointer(texture0Ptr),
                   Eq(std::nullopt), Eq(std::nullopt))
    ).WillOnce(Return(success));

    system.renderTextureInFullWindow(textureId0);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture,
       throwIfSDLRenderClearFailedDuringFrameRendering)
{
    EXPECT_CALL(*sdlMock, renderPresent(Pointer(rendererPtr)));
    EXPECT_CALL(*sdlMock, renderClear(Pointer(rendererPtr))).WillOnce(Return(failure));
    EXPECT_THROW(system.renderFrame(), std::runtime_error);
    expectQuitSystemWithLoadedTexture();
}

TEST_F(SDLGraphicsSystemWithLoadedTexture, renderFrameSuccessfully) {
    EXPECT_CALL(*sdlMock, renderPresent(Pointer(rendererPtr)));
    EXPECT_CALL(*sdlMock, renderClear(Pointer(rendererPtr))).WillOnce(Return(success));
    system.renderFrame();
    expectQuitSystemWithLoadedTexture();
}

}
