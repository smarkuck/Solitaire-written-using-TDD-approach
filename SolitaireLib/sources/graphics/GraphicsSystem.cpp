#include <stdexcept>

#include "graphics/GraphicsSystem.h"
#include "graphics/SDLWrapper.h"

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

SDLPtr<SDL_Renderer>
GraphicsSystem::createSDLWindowRendererOrQuitAndThrowError(const SDLPtr<SDL_Window>& window) {
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
    renderer.reset();
    window.reset();

    if (isSDLInitialized)
    {
        isSDLInitialized = false;
        SDL->quit();
    }
}

}
