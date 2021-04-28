#include <cassert>

#include "Renderer.h"
#include "SDLWrapper.h"

namespace solitaire {

const std::string Renderer::assetsPath {"../../../assets/"};

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

SDL_Texture* Renderer::loadTexture(const std::string& path) const
{
    SDL_Surface* surface = sdl.loadBMP(assetsPath + path);
    assert(surface != nullptr);
    assert(sdl.setColorKey(surface, SDL_TRUE, sdl.mapRGB(surface->format, 0, 255, 255)) == 0);

    SDL_Texture* texture = sdl.createTextureFromSurface(renderer, surface);
    assert(texture != nullptr);
    sdl.freeSurface(surface);

    return texture;
}

Renderer::~Renderer() {
    sdl.delay(2000);

    for (const auto& texture: cards)
        sdl.destroyTexture(texture);
    sdl.destroyTexture(cardBack);
    sdl.destroyTexture(cardPlaceholder);
    sdl.destroyTexture(background);
    sdl.destroyRenderer(renderer);
    sdl.destroyWindow(window);
    sdl.quit();
}

void Renderer::render() const {
    sdl.setRenderDrawColor(renderer, 0, 0, 0, 0);
    sdl.renderClear(renderer);
    sdl.renderCopy(renderer, background, nullptr, nullptr);

    for (int i = 0; i < 8; i++) {
        SDL_Rect rect {i*75, 0, 75, 104};
        sdl.renderCopy(renderer, cards[i], nullptr, &rect);
    }

    for (int i = 0; i < 4; i++) {
        SDL_Rect rect {i*75, 104, 75, 104};
        sdl.renderCopy(renderer, cardPlaceholder, nullptr, &rect);
    }

    SDL_Rect rect {0, 208, 75, 104};
    sdl.renderCopy(renderer, cardBack, nullptr, &rect);

    sdl.renderPresent(renderer);
}

}
