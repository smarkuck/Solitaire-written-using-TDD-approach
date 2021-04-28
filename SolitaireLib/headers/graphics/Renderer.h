#pragma once

#include <array>
#include <string>

#include "SDLWrapper.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire {

class Solitaire;

namespace graphics {

class DefaultSDLDeleter;

class Renderer {
public:
    Renderer(const Solitaire&, const SDLWrapper<DefaultSDLDeleter>&);
    ~Renderer();

    void render() const;

private:
    static std::string findAssetsPath();

    static constexpr unsigned cardsQuantity {52};
    static const std::string assetsPath;

    SDLPtr<SDL_Texture> loadTexture(const std::string& path) const;

    const Solitaire& solitaire;
    const SDLWrapper<DefaultSDLDeleter>& sdl;
    SDLPtr<SDL_Window> window;
    SDLPtr<SDL_Renderer> renderer;
    std::array<SDLPtr<SDL_Texture>, cardsQuantity> cards;
    SDLPtr<SDL_Texture> cardBack;
    SDLPtr<SDL_Texture> cardPlaceholder;
    SDLPtr<SDL_Texture> background;
};

}
}
