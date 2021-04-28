#pragma once

#include "SDL.h"

namespace solitaire::graphics {

class SDLDeleter {
public:
    void operator() (SDL_Window* window) const {
        SDL_DestroyWindow(window);
    }

    void operator() (SDL_Renderer* renderer) const {
        SDL_DestroyRenderer(renderer);
    }

    void operator() (SDL_Texture* texture) const {
        SDL_DestroyTexture(texture);
    }

    void operator() (SDL_Surface* surface) const {
        SDL_FreeSurface(surface);
    }
};

}
