#include <stdexcept>

#include "graphics/SDLGraphicsSystem.h"
#include "graphics/TextureId.h"
#include "graphics/TextureArea.h"
#include "SDL/Wrapper.h"

using namespace solitaire::SDL;

namespace solitaire::graphics {

SDLGraphicsSystem::SDLGraphicsSystem(std::unique_ptr<Wrapper> sdl):
    sdl {std::move(sdl)} {
}

SDLGraphicsSystem::~SDLGraphicsSystem() {
    quit();
}

void SDLGraphicsSystem::createWindow(
    const std::string& title, const unsigned width, const unsigned height)
{
    if (isWindowCreated)
        throw std::runtime_error {"Window already created"};

    initializeSDLOrQuitAndThrowError();
    window = createSDLWindowOrQuitAndThrowError(title, width, height);
    renderer = createSDLWindowRendererOrQuitAndThrowError(window);
    isWindowCreated = true;
}

void SDLGraphicsSystem::initializeSDLOrQuitAndThrowError() {
    if (sdl->init(SDL_INIT_VIDEO))
        quitAndThrow("Cannot initialize graphics system");

    isSDLInitialized = true;
}

UniquePtr<SDL_Window> SDLGraphicsSystem::createSDLWindowOrQuitAndThrowError(
    const std::string& title, const unsigned width, const unsigned height)
{
    auto window = sdl->createWindow(
        title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
    );

    if (not window)
        quitAndThrow("Cannot create window");
    return window;
}

UniquePtr<SDL_Renderer> SDLGraphicsSystem::createSDLWindowRendererOrQuitAndThrowError(
    const UniquePtr<SDL_Window>& window)
{
    auto renderer = sdl->createRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (not renderer)
        quitAndThrow("Cannot create window renderer");
    return renderer;
}

void SDLGraphicsSystem::quitAndThrow(const std::string& error) {
    quit();
    throw std::runtime_error {error};
}

void SDLGraphicsSystem::quit() {
    textures.clear();
    renderer.reset();
    window.reset();

    if (isSDLInitialized)
    {
        isSDLInitialized = false;
        sdl->quit();
    }
}

TextureId SDLGraphicsSystem::loadTexture(const std::string& path) {
    if (not isWindowCreated)
        throw std::runtime_error {"Cannot load texture when window not created"};

    textures.emplace_back(createSDLTextureOrThrow(createSDLSurfaceOrThrow(path)));

    return TextureId {static_cast<unsigned>(textures.size() - 1)};
}

UniquePtr<SDL_Surface>
SDLGraphicsSystem::createSDLSurfaceOrThrow(const std::string& path) const {
    auto surface = sdl->loadBMP(path);

    if (not surface)
        throw std::runtime_error {"Cannot find texture " + path};

    if (sdl->setColorKey(surface, SDL_TRUE, sdl->mapRGB(surface, 0, 255, 255)))
        throw std::runtime_error {"Cannot set transparent color for " + path};

    return surface;
}

UniquePtr<SDL_Texture> SDLGraphicsSystem::createSDLTextureOrThrow(
    const UniquePtr<SDL_Surface>& surface) const
{
    auto texture = sdl->createTextureFromSurface(renderer, surface);

    if (not texture)
        throw std::runtime_error {"Cannot create texture from surface"};
    return texture;
}

void SDLGraphicsSystem::setTextureAlpha(const TextureId id, const uint8_t alpha) const {
    throwOnInvalidTextureOperation(id);
    if (sdl->setTextureAlphaMod(textures[id], alpha))
        throw std::runtime_error {"Cannot change alpha for texture with id: " + id};
}

void SDLGraphicsSystem::renderTexture(
    const TextureId id, const TexturePosition& position, const TextureArea& area) const
{
    throwOnInvalidTextureOperation(id);
    const auto srcRect = createSrcRect(area);
    const auto dstRect = createDstRect(position, area);

    if (sdl->renderCopy(renderer, textures[id], srcRect, dstRect))
        throw std::runtime_error {"Cannot render texture with id: " + id};
}

SDL_Rect SDLGraphicsSystem::createSrcRect(const TextureArea& area) const {
    return SDL_Rect {
        static_cast<int>(area.position.x), static_cast<int>(area.position.y),
        static_cast<int>(area.size.width), static_cast<int>(area.size.height)
    };
}

SDL_Rect SDLGraphicsSystem::createDstRect(
    const TexturePosition& position, const TextureArea& area) const
{
    return SDL_Rect {
        static_cast<int>(position.x), static_cast<int>(position.y),
        static_cast<int>(area.size.width), static_cast<int>(area.size.height)
    };
}

void SDLGraphicsSystem::renderTextureInFullWindow(const TextureId id) const {
    throwOnInvalidTextureOperation(id);
     if (sdl->renderCopy(renderer, textures[id], std::nullopt, std::nullopt))
        throw std::runtime_error {"Cannot render texture with id: " + id};
}

void SDLGraphicsSystem::throwOnInvalidTextureOperation(const TextureId id) const {
    if (not isWindowCreated)
        throw std::runtime_error {"Cannot operate on textures when window not created"};

    if (id.t >= textures.size())
        throw std::runtime_error {"Unknown texture id: " + id};
}

void SDLGraphicsSystem::renderFrame() const {
    if (not isWindowCreated)
        throw std::runtime_error {"Cannot render frame when window not created"};

    sdl->renderPresent(renderer);

    if (sdl->renderClear(renderer))
        throw std::runtime_error {"Cannot clear frame"};
}

}
