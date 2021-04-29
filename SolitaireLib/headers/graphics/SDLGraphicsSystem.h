#pragma once

#include <memory>
#include <vector>

#include "GraphicsSystem.h"
#include "SDLPtr.h"

struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire::graphics {

class SDLWrapper;

class SDLGraphicsSystem: public GraphicsSystem {
public:
    SDLGraphicsSystem(std::unique_ptr<SDLWrapper>);
    ~SDLGraphicsSystem();

    void createWindow(const std::string& title,
                      const unsigned width, const unsigned height) override;

    TextureId loadTexture(const std::string& path) override;

    void setTextureAlpha(const TextureId, const uint8_t alpha) override;
    void renderTexture(const TextureId, const TexturePosition&,
                       const TextureArea&) override;
    void renderTextureOnFullscreen(const TextureId) override;
    void renderFrame() override;

private:
    void initializeSDLOrQuitAndThrowError();

    SDLPtr<SDL_Window> createSDLWindowOrQuitAndThrowError(
        const std::string& title, const unsigned width, const unsigned height);

    SDLPtr<SDL_Renderer>
    createSDLWindowRendererOrQuitAndThrowError(const SDLPtr<SDL_Window>&);

    void quitAndThrow(const std::string& error);
    void quit();

    SDLPtr<SDL_Surface> createSDLSurfaceOrThrow(const std::string& path) const;
    SDLPtr<SDL_Texture> createSDLTextureOrThrow(const SDLPtr<SDL_Surface>&) const;

    SDL_Rect createSrcRect(const TextureArea&) const;
    SDL_Rect createDstRect(const TexturePosition&, const TextureArea&) const;

    void throwOnInvalidTextureOperation(const TextureId) const;

    std::unique_ptr<SDLWrapper> SDL;
    SDLPtr<SDL_Window> window;
    SDLPtr<SDL_Renderer> renderer;
    std::vector<SDLPtr<SDL_Texture>> textures;

    bool isSDLInitialized {false};
    bool isWindowCreated {false};
};

}
