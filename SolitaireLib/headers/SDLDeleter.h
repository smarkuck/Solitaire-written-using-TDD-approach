#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

namespace solitaire {

class SDLDeleter {
public:
    virtual ~SDLDeleter() = default;
    virtual void operator() (SDL_Window*) const = 0;
    virtual void operator() (SDL_Renderer*) const = 0;
    virtual void operator() (SDL_Texture*) const = 0;
    virtual void operator() (SDL_Surface*) const = 0;
};

}
