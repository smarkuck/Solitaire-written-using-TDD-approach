#pragma once

#include <memory>
#include <vector>

#include "interfaces/graphics/GraphicsSystem.h"
#include "SDL/UniquePtr.h"

struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire::SDL::interfaces {
class Wrapper;
}

namespace solitaire::graphics {

class SDLGraphicsSystem: public interfaces::GraphicsSystem {
public:
    SDLGraphicsSystem(std::unique_ptr<SDL::interfaces::Wrapper>);
    ~SDLGraphicsSystem();

    void createWindow(const std::string& title, const unsigned width,
                      const unsigned height) override;

    TextureId loadTexture(const std::string& path) override;

    void setTextureAlpha(const TextureId, const uint8_t alpha) const override;
    void renderTexture(const TextureId, const geometry::Position&,
                       const geometry::Area&) const override;
    void renderTextureInFullWindow(const TextureId) const override;
    void renderFrame() const override;

private:
    void initializeSDLOrQuitAndThrowError();

    SDL::UniquePtr<SDL_Window> createSDLWindowOrQuitAndThrowError(
        const std::string& title, const unsigned width, const unsigned height);

    SDL::UniquePtr<SDL_Renderer> createSDLWindowRendererOrQuitAndThrowError(
        const SDL::UniquePtr<SDL_Window>&);

    void quitAndThrow(const std::string& error);
    void quit();

    SDL::UniquePtr<SDL_Surface> createSDLSurfaceOrThrow(const std::string& path) const;
    SDL::UniquePtr<SDL_Texture> createSDLTextureOrThrow(
        const SDL::UniquePtr<SDL_Surface>&) const;

    SDL_Rect createSrcRect(const geometry::Area&) const;
    SDL_Rect createDstRect(const geometry::Position&, const geometry::Area&) const;

    void throwOnInvalidTextureOperation(const TextureId) const;

    std::unique_ptr<SDL::interfaces::Wrapper> sdl;
    SDL::UniquePtr<SDL_Window> window;
    SDL::UniquePtr<SDL_Renderer> renderer;
    std::vector<SDL::UniquePtr<SDL_Texture>> textures;

    bool isSDLInitialized {false};
    bool isWindowCreated {false};
};

}
