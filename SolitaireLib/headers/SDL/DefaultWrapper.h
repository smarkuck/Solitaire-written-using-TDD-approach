#pragma once

#include "interfaces/SDL/Wrapper.h"

namespace solitaire::SDL {

class DefaultWrapper: public interfaces::Wrapper {
public:
    int init(Uint32 flags) const override;

    void quit() const override;

    int pollEvent(SDL_Event&) const override;

    UniquePtr<SDL_Window> createWindow(const std::string& title, int x, int y, int w, int h,
                                       Uint32 flags) const override;

    UniquePtr<SDL_Renderer> createRenderer(const UniquePtr<SDL_Window>&,
                                           int index, Uint32 flags) const override;

    UniquePtr<SDL_Texture> createTextureFromSurface(
        const UniquePtr<SDL_Renderer>&, const UniquePtr<SDL_Surface>&) const override;

    UniquePtr<SDL_Surface> loadBMP(const std::string& file) const override;

    int setTextureAlphaMod(const UniquePtr<SDL_Texture>&, Uint8 alpha) const override;

    int setColorKey(const UniquePtr<SDL_Surface>&, int flag, Uint32 key) const override;

    Uint32 mapRGB(const UniquePtr<SDL_Surface>&, Uint8 r, Uint8 g, Uint8 b) const override;

    int setRenderDrawColor(const UniquePtr<SDL_Renderer>&,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a) const override;

    int renderClear(const UniquePtr<SDL_Renderer>&) const override;

    int renderCopy(const UniquePtr<SDL_Renderer>&, const UniquePtr<SDL_Texture>&,
                   const std::optional<SDL_Rect>& srcrect,
                   const std::optional<SDL_Rect>& dstrect) const override;

    void renderPresent(const UniquePtr<SDL_Renderer>&) const override;

    void delay(Uint32 ms) const override;
};

}
