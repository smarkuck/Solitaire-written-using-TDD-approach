#include <stdexcept>

#include "graphics/GraphicsSystem.h"
#include "graphics/SDLWrapper.h"

namespace solitaire::graphics {

GraphicsSystem::GraphicsSystem(std::unique_ptr<SDLWrapper> SDL):
    SDL {std::move(SDL)} {
}

GraphicsSystem::~GraphicsSystem() {
    renderer.reset();
    window.reset();

    if (isSDLInitialized)
        SDL->quit();
}

void GraphicsSystem::createWindow(
    const std::string& title, unsigned width, unsigned height)
{
    if (SDL->init(SDL_INIT_VIDEO))
        throw std::runtime_error {"Cannot initialize graphics system"};

    isSDLInitialized = true;

    window = SDL->createWindow(
        title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
    );

    if (not window)
        throw std::runtime_error {"Cannot create window"};

    renderer = SDL->createRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (not renderer)
        throw std::runtime_error {"Cannot create window renderer"};
}

}
