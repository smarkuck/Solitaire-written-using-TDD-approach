#include "SDL.h"
#include "SDL/PtrDeleter.h"

namespace solitaire::SDL {

void PtrDeleter::operator() (SDL_Window* window) const {
    SDL_DestroyWindow(window);
}

void PtrDeleter::operator() (SDL_Renderer* renderer) const {
    SDL_DestroyRenderer(renderer);
}

void PtrDeleter::operator() (SDL_Texture* texture) const {
    SDL_DestroyTexture(texture);
}

void PtrDeleter::operator() (SDL_Surface* surface) const {
    SDL_FreeSurface(surface);
}

}
