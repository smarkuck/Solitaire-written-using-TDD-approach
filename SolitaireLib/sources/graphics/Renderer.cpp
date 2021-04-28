#include <cassert>
#include <filesystem>

#include "graphics/Renderer.h"
#include "graphics/SDLWrapper.h"

namespace solitaire::graphics {

const std::string Renderer::assetsPath {findAssetsPath()};

std::string Renderer::findAssetsPath() {
    std::string path = "assets/";
    const std::string moveUp = "../";

    for (unsigned i = 0; i < 4; i++, path = moveUp + path)
        if (std::filesystem::exists(path))
            return path;

    assert(false and "cannot find assets path");
    return {};
}

Renderer::Renderer(const Solitaire& solitaire, const SDLWrapper& sdl):
    solitaire {solitaire},
    sdl {sdl}
{
    sdl.init(SDL_INIT_VIDEO);

    window = sdl.createWindow("Solitaire",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480, SDL_WINDOW_SHOWN
    );
    assert(window != nullptr);

    renderer = sdl.createRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(renderer != nullptr);

    background = loadTexture("background.bmp");
    cardPlaceholder = loadTexture("cards/placeholder.bmp");
    assert(sdl.setTextureAlphaMod(cardPlaceholder, 70) == 0);

    for (unsigned i = 0; i < cardsQuantity; i++)
        cards[i] = loadTexture("cards/" + std::to_string(i) + ".bmp");
    cardBack = loadTexture("cards/back.bmp");
}

SDLPtr<SDL_Texture> Renderer::loadTexture(const std::string& path) const
{
    auto surface = sdl.loadBMP(assetsPath + path);
    assert(surface != nullptr);
    assert(sdl.setColorKey(surface, SDL_TRUE, sdl.mapRGB(surface, 0, 255, 255)) == 0);

    auto texture = sdl.createTextureFromSurface(renderer, surface);
    assert(texture != nullptr);

    return texture;
}

Renderer::~Renderer() {
    sdl.delay(2000);
    sdl.quit();
}

void Renderer::render() const {
    sdl.setRenderDrawColor(renderer, 0, 0, 0, 0);
    sdl.renderClear(renderer);
    sdl.renderCopy(renderer, background, std::nullopt, std::nullopt);

    for (int i = 0; i < 8; i++) {
        SDL_Rect rect {i*75, 0, 75, 104};
        sdl.renderCopy(renderer, cards[i], std::nullopt, rect);
    }

    for (int i = 0; i < 4; i++) {
        SDL_Rect rect {i*75, 104, 75, 104};
        sdl.renderCopy(renderer, cardPlaceholder, std::nullopt, rect);
    }

    SDL_Rect rect {0, 208, 75, 104};
    sdl.renderCopy(renderer, cardBack, std::nullopt, rect);

    sdl.renderPresent(renderer);
}

}
