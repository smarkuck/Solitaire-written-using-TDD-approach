#pragma once

#include "gmock/gmock.h"
#include "SDL/Wrapper.h"

namespace solitaire::SDL {

class WrapperMock: public Wrapper {
public:
    MOCK_METHOD(int, init, (Uint32), (const, override));

    MOCK_METHOD(void, quit, (), (const, override));

    MOCK_METHOD(int, pollEvent, (SDL_Event&), (const, override));

    MOCK_METHOD(UniquePtr<SDL_Window>, createWindow,
                (const std::string&, int, int, int, int, Uint32), (const, override));

    MOCK_METHOD(UniquePtr<SDL_Renderer>, createRenderer,
                (const UniquePtr<SDL_Window>&, int, Uint32), (const, override));

    MOCK_METHOD(UniquePtr<SDL_Texture>, createTextureFromSurface,
                (const UniquePtr<SDL_Renderer>&, const UniquePtr<SDL_Surface>&),
                (const, override));

    MOCK_METHOD(UniquePtr<SDL_Surface>, loadBMP, (const std::string&), (const, override));

    MOCK_METHOD(int, setTextureAlphaMod,
                (const UniquePtr<SDL_Texture>&, Uint8), (const, override));

    MOCK_METHOD(int, setColorKey,
                (const UniquePtr<SDL_Surface>&, int, Uint32), (const, override));

    MOCK_METHOD(Uint32, mapRGB,
                (const UniquePtr<SDL_Surface>&, Uint8, Uint8, Uint8), (const, override));

    MOCK_METHOD(int, setRenderDrawColor,
                (const UniquePtr<SDL_Renderer>&, Uint8, Uint8, Uint8, Uint8),
                (const, override));

    MOCK_METHOD(int, renderClear, (const UniquePtr<SDL_Renderer>&), (const, override));

    MOCK_METHOD(int, renderCopy,
                (const UniquePtr<SDL_Renderer>&, const UniquePtr<SDL_Texture>&,
                 const std::optional<SDL_Rect>&, const std::optional<SDL_Rect>&),
                 (const, override));

    MOCK_METHOD(void, renderPresent, (const UniquePtr<SDL_Renderer>&), (const, override));

    MOCK_METHOD(void, delay, (Uint32), (const, override));
};

}
