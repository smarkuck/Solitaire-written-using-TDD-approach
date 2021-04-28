#pragma once

#include "gmock/gmock.h"
#include "graphics/SDLWrapper.h"

namespace solitaire::graphics {

class SDLWrapperMock: public SDLWrapper {
public:
    MOCK_METHOD(int, init, (Uint32), (const));

    MOCK_METHOD(void, quit, (), (const));

    MOCK_METHOD(SDLPtr<SDL_Window>, createWindow,
                (const std::string&, int, int, int, int, Uint32), (const));

    MOCK_METHOD(SDLPtr<SDL_Renderer>, createRenderer,
                (const SDLPtr<SDL_Window>&, int, Uint32), (const));

    MOCK_METHOD(SDLPtr<SDL_Texture>, createTextureFromSurface,
                (const SDLPtr<SDL_Renderer>&, const SDLPtr<SDL_Surface>&), (const));

    MOCK_METHOD(SDLPtr<SDL_Surface>, loadBMP, (const std::string&), (const));

    MOCK_METHOD(int, setTextureAlphaMod, (const SDLPtr<SDL_Texture>&, Uint8), (const));

    MOCK_METHOD(int, setColorKey, (const SDLPtr<SDL_Surface>&, int, Uint32), (const));

    MOCK_METHOD(Uint32, mapRGB, (const SDLPtr<SDL_Surface>&, Uint8, Uint8, Uint8), (const));

    MOCK_METHOD(int, setRenderDrawColor,
                (const SDLPtr<SDL_Renderer>&, Uint8, Uint8, Uint8, Uint8), (const));

    MOCK_METHOD(int, renderClear, (const SDLPtr<SDL_Renderer>&), (const));

    MOCK_METHOD(int, renderCopy,
                (const SDLPtr<SDL_Renderer>&, const SDLPtr<SDL_Texture>&,
                 const std::optional<SDL_Rect>&, const std::optional<SDL_Rect>&), (const));

    MOCK_METHOD(void, renderPresent, (const SDLPtr<SDL_Renderer>&), (const));

    MOCK_METHOD(void, delay, (Uint32), (const));
};

}
