#pragma once

#include <optional>
#include <string>

#include "SDL.h"
#include "SDLPtr.h"

namespace solitaire {

template <class Deleter>
class SDLWrapper {
public:
    SDLWrapper(std::unique_ptr<Deleter> deleter):
        deleter {std::move(deleter)} {
    }

    int init(Uint32 flags) const {
        return SDL_Init(flags);
    }

    void quit() const {
        SDL_Quit();
    }

    SDLPtr<SDL_Window> createWindow(const std::string& title,
                                    int x, int y, int w, int h, Uint32 flags) const
    {
        return SDLPtr<SDL_Window> {
            SDL_CreateWindow(title.c_str(), x, y, w, h, flags), *deleter};
    }

    SDLPtr<SDL_Renderer> createRenderer(const SDLPtr<SDL_Window>& window,
                                        int index, Uint32 flags) const
    {
        return SDLPtr<SDL_Renderer> {
            SDL_CreateRenderer(window.get(), index, flags), *deleter};
    }

    SDLPtr<SDL_Texture> createTextureFromSurface(const SDLPtr<SDL_Renderer>& renderer,
                                                 const SDLPtr<SDL_Surface>& surface) const {
        return SDLPtr<SDL_Texture> {
            SDL_CreateTextureFromSurface(renderer.get(), surface.get()), *deleter};
    }

    SDLPtr<SDL_Surface> loadBMP(const std::string& file) const {
        return SDLPtr<SDL_Surface> {SDL_LoadBMP(file.c_str()), *deleter};
    }

    int setTextureAlphaMod(const SDLPtr<SDL_Texture>& texture, Uint8 alpha) const {
        return SDL_SetTextureAlphaMod(texture.get(), alpha);
    }

    int setColorKey(const SDLPtr<SDL_Surface>& surface, int flag, Uint32 key) const {
        return SDL_SetColorKey(surface.get(), flag, key);
    }

    Uint32 mapRGB(const SDLPtr<SDL_Surface>& surface, Uint8 r, Uint8 g, Uint8 b) const {
        return SDL_MapRGB(surface->format, r, g, b);
    }

    int setRenderDrawColor(const SDLPtr<SDL_Renderer>& renderer,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        return SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
    }

    int renderClear(const SDLPtr<SDL_Renderer>& renderer) const {
        return SDL_RenderClear(renderer.get());
    }

    int renderCopy(const SDLPtr<SDL_Renderer>& renderer,
                   const SDLPtr<SDL_Texture>& texture,
                   const std::optional<SDL_Rect>& srcrect,
                   const std::optional<SDL_Rect>& dstrect) const
    {
        const SDL_Rect* src = srcrect ? &srcrect.value() : nullptr;
        const SDL_Rect* dst = dstrect ? &dstrect.value() : nullptr;
        return SDL_RenderCopy(renderer.get(), texture.get(), src, dst);
    }

    void renderPresent(const SDLPtr<SDL_Renderer>& renderer) const {
        SDL_RenderPresent(renderer.get());
    }

    void delay(Uint32 ms) const {
        SDL_Delay(ms);
    }

private:
    std::unique_ptr<Deleter> deleter;
};

}
