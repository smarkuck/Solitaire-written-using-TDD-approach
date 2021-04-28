#pragma once

#include <string>
#include "SDL.h"

namespace solitaire {

class SDLWrapper {
public:
    int init(Uint32 flags) const;
    void quit() const;

    SDL_Window* createWindow(const std::string& title,
                             int x, int y, int w, int h, Uint32 flags) const;
    void destroyWindow(SDL_Window*) const;

    SDL_Renderer* createRenderer(SDL_Window*, int index, Uint32 flags) const;
    void destroyRenderer(SDL_Renderer*) const;

    SDL_Texture* createTextureFromSurface(SDL_Renderer*, SDL_Surface*) const;
    void destroyTexture(SDL_Texture*) const;

    SDL_Surface* loadBMP(const std::string& file) const;
    void freeSurface(SDL_Surface*) const;

    int setTextureAlphaMod(SDL_Texture*, Uint8 alpha) const;
    int setColorKey(SDL_Surface*, int flag, Uint32 key) const;
    Uint32 mapRGB(const SDL_PixelFormat*, Uint8 r, Uint8 g, Uint8 b) const;

    int setRenderDrawColor(SDL_Renderer*, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;
    int renderClear(SDL_Renderer*) const;
    int renderCopy(SDL_Renderer*, SDL_Texture*,
                   const SDL_Rect* srcrect, const SDL_Rect* dstrect) const;
    void renderPresent(SDL_Renderer*) const;

    void delay(Uint32 ms) const;
};

}
