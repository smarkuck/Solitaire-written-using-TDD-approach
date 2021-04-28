#pragma once

#include "SDL.h"
#include "SDLDeleter.h"

namespace solitaire::graphics {

class DefaultSDLDeleter: public SDLDeleter {
public:
    void operator() (SDL_Window* window) const override {
        SDL_DestroyWindow(window);
    }

    void operator() (SDL_Renderer* renderer) const override {
        SDL_DestroyRenderer(renderer);
    }

    void operator() (SDL_Texture* texture) const override {
        SDL_DestroyTexture(texture);
    }

    void operator() (SDL_Surface* surface) const override {
        SDL_FreeSurface(surface);
    }
};

}
