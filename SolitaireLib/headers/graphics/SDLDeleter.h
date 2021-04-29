#pragma once

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire::graphics {

class SDLDeleter {
public:
    void operator() (SDL_Window* window) const;
    void operator() (SDL_Renderer* renderer) const;
    void operator() (SDL_Texture* texture) const;
    void operator() (SDL_Surface* surface) const;
};

}
