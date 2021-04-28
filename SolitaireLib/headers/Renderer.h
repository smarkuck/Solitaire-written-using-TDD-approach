#pragma once

#include <array>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire {

class SDLWrapper;
class Solitaire;

class Renderer {
public:
    Renderer(const Solitaire&, const SDLWrapper&);
    ~Renderer();

    void render() const;

private:
    SDL_Texture* loadTexture(const std::string& path) const;

    static constexpr unsigned cardsQuantity {52};
    static const std::string assetsPath;

    const Solitaire& solitaire;
    const SDLWrapper& sdl;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::array<SDL_Texture*, cardsQuantity> cards;
    SDL_Texture* cardBack;
    SDL_Texture* cardPlaceholder;
    SDL_Texture* background;
};

}
