#include <stdexcept>

#include "graphics/GraphicsSystem.h"
#include "graphics/SDLWrapper.h"
#include "graphics/TextureId.h"

namespace solitaire::graphics {

GraphicsSystem::GraphicsSystem(std::unique_ptr<SDLWrapper> SDL):
    SDL {std::move(SDL)} {
}

GraphicsSystem::~GraphicsSystem() {
    quit();
}

void GraphicsSystem::createWindow(
    const std::string& title, unsigned width, unsigned height)
{
    if (isWindowCreated)
        throw std::runtime_error {"Window already created"};

    initializeSDLOrQuitAndThrowError();
    window = createSDLWindowOrQuitAndThrowError(title, width, height);
    renderer = createSDLWindowRendererOrQuitAndThrowError(window);
    isWindowCreated = true;
}

void GraphicsSystem::initializeSDLOrQuitAndThrowError() {
    if (SDL->init(SDL_INIT_VIDEO))
        quitAndThrow("Cannot initialize graphics system");

    isSDLInitialized = true;
}

SDLPtr<SDL_Window> GraphicsSystem::createSDLWindowOrQuitAndThrowError(
    const std::string& title, unsigned width, unsigned height)
{
    auto window = SDL->createWindow(
        title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
    );

    if (not window)
        quitAndThrow("Cannot create window");
    return window;
}

SDLPtr<SDL_Renderer> GraphicsSystem::createSDLWindowRendererOrQuitAndThrowError(
    const SDLPtr<SDL_Window>& window)
{
    auto renderer = SDL->createRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (not renderer)
        quitAndThrow("Cannot create window renderer");
    return renderer;
}

void GraphicsSystem::quitAndThrow(const std::string& error) {
    quit();
    throw std::runtime_error {error};
}

void GraphicsSystem::quit() {
    textures.clear();
    renderer.reset();
    window.reset();

    if (isSDLInitialized)
    {
        isSDLInitialized = false;
        SDL->quit();
    }
}

TextureId GraphicsSystem::loadTexture(
    const std::string& path, unsigned width, unsigned height)
{
    if (not isWindowCreated)
        throw std::runtime_error {"Cannot load texture when window not created"};

    textures.push_back(TextureData {
        createSDLTextureOrThrow(createSDLSurfaceOrThrow(path)),
        width, height
    });

    return TextureId {static_cast<unsigned>(textures.size() - 1)};
}

SDLPtr<SDL_Surface>
GraphicsSystem::createSDLSurfaceOrThrow(const std::string& path) const {
    auto surface = SDL->loadBMP(path);

    if (not surface)
        throw std::runtime_error {"Cannot find texture " + path};

    if (SDL->setColorKey(surface, SDL_TRUE, SDL->mapRGB(surface, 0, 255, 255)))
        throw std::runtime_error {"Cannot set transparent color for " + path};

    return surface;
}

SDLPtr<SDL_Texture> GraphicsSystem::createSDLTextureOrThrow(
    const SDLPtr<SDL_Surface>& surface) const
{
    auto texture = SDL->createTextureFromSurface(renderer, surface);

    if (not texture)
        throw std::runtime_error {"Cannot create texture from surface"};
    return texture;
}

void GraphicsSystem::setTextureAlpha(TextureId textureId, uint8_t alpha) {
    if (not isWindowCreated)
        throw std::runtime_error {"Cannot set texture alpha when window not created"};

    if (textureId.t >= textures.size())
        throw std::runtime_error {"Unknown texture id: " + textureId};

    if (SDL->setTextureAlphaMod(textures[textureId].texture, 70))
        throw std::runtime_error {
            "Cannot change alpha for texture with id: " + textureId};
}

}
