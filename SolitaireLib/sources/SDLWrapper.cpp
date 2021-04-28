#include "SDLWrapper.h"

namespace solitaire {

int SDLWrapper::init(Uint32 flags) const {
    return SDL_Init(flags);
}

void SDLWrapper::quit() const {
    SDL_Quit();
}

SDL_Window* SDLWrapper::createWindow(const std::string& title,
                                     int x, int y, int w, int h, Uint32 flags) const
{
    return SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
}

void SDLWrapper::destroyWindow(SDL_Window* window) const {
    SDL_DestroyWindow(window);
}

SDL_Renderer* SDLWrapper::createRenderer(
    SDL_Window* window, int index, Uint32 flags) const
{
    return SDL_CreateRenderer(window, index, flags);
}

void SDLWrapper::destroyRenderer(SDL_Renderer* renderer) const {
    SDL_DestroyRenderer(renderer);
}

SDL_Texture* SDLWrapper::createTextureFromSurface(
    SDL_Renderer* renderer, SDL_Surface* surface) const
{
    return SDL_CreateTextureFromSurface(renderer, surface);
}

void SDLWrapper::destroyTexture(SDL_Texture* texture) const {
    SDL_DestroyTexture(texture);
}

SDL_Surface* SDLWrapper::loadBMP(const std::string& file) const {
    return SDL_LoadBMP(file.c_str());
}

void SDLWrapper::freeSurface(SDL_Surface* surface) const {
    SDL_FreeSurface(surface);
}

int SDLWrapper::setTextureAlphaMod(SDL_Texture* texture, Uint8 alpha) const {
    return SDL_SetTextureAlphaMod(texture, alpha);
}

int SDLWrapper::setColorKey(SDL_Surface* surface, int flag, Uint32 key) const {
    return SDL_SetColorKey(surface, flag, key);
}

Uint32 SDLWrapper::mapRGB(const SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b) const {
    return SDL_MapRGB(format, r, g, b);
}

int SDLWrapper::setRenderDrawColor(SDL_Renderer* renderer,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

int SDLWrapper::renderClear(SDL_Renderer* renderer) const {
    return SDL_RenderClear(renderer);
}

int SDLWrapper::renderCopy(SDL_Renderer* renderer, SDL_Texture* texture,
                           const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
    return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
}

void SDLWrapper::renderPresent(SDL_Renderer* renderer) const {
    SDL_RenderPresent(renderer);
}

void SDLWrapper::delay(Uint32 ms) const {
    SDL_Delay(ms);
}

}
