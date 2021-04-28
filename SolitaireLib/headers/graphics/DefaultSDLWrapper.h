#pragma once

#include "SDLWrapper.h"

namespace solitaire::graphics {

class DefaultSDLWrapper: public SDLWrapper {
public:
    int init(Uint32 flags) const override;

    void quit() const override;

    SDLPtr<SDL_Window> createWindow(const std::string& title,
                                    int x, int y, int w, int h, Uint32 flags) const override;

    SDLPtr<SDL_Renderer> createRenderer(const SDLPtr<SDL_Window>&,
                                        int index, Uint32 flags) const override;

    SDLPtr<SDL_Texture> createTextureFromSurface(
        const SDLPtr<SDL_Renderer>&, const SDLPtr<SDL_Surface>&) const override;

    SDLPtr<SDL_Surface> loadBMP(const std::string& file) const override;

    int setTextureAlphaMod(const SDLPtr<SDL_Texture>&, Uint8 alpha) const override;

    int setColorKey(const SDLPtr<SDL_Surface>&, int flag, Uint32 key) const override;

    Uint32 mapRGB(const SDLPtr<SDL_Surface>&, Uint8 r, Uint8 g, Uint8 b) const override;

    int setRenderDrawColor(const SDLPtr<SDL_Renderer>&,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a) const override;

    int renderClear(const SDLPtr<SDL_Renderer>&) const override;

    int renderCopy(const SDLPtr<SDL_Renderer>&, const SDLPtr<SDL_Texture>&,
                   const std::optional<SDL_Rect>& srcrect,
                   const std::optional<SDL_Rect>& dstrect) const override;

    void renderPresent(const SDLPtr<SDL_Renderer>&) const override;

    void delay(Uint32 ms) const override;
};

}
