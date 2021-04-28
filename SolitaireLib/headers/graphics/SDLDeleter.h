#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

namespace solitaire::graphics {

class SDLDeleter {
public:
    void operator() (SDL_Window* window) const;
    void operator() (SDL_Renderer* renderer) const;
    void operator() (SDL_Texture* texture) const;
    void operator() (SDL_Surface* surface) const;
};

}
