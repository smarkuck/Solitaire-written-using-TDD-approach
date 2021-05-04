#pragma once

#include "gmock/gmock.h"

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire::SDL {

class PtrDeleterMock {
public:
    MOCK_METHOD(void, windowDeleter, (SDL_Window*), (const));
    MOCK_METHOD(void, rendererDeleter, (SDL_Renderer*), (const));
    MOCK_METHOD(void, textureDeleter, (SDL_Texture*), (const));
    MOCK_METHOD(void, surfaceDeleter, (SDL_Surface*), (const));
};

class PtrDeleter {
public:
    PtrDeleter(std::shared_ptr<PtrDeleterMock> mock):
        mock {std::move(mock)} {
    }

    void operator() (SDL_Window* window) const { mock->windowDeleter(window); }
    void operator() (SDL_Renderer* renderer) const { mock->rendererDeleter(renderer); }
    void operator() (SDL_Texture* texture) const { mock->textureDeleter(texture); }
    void operator() (SDL_Surface* surface) const { mock->surfaceDeleter(surface); }

private:
    std::shared_ptr<PtrDeleterMock> mock;
};

}
