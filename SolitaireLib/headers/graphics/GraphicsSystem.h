#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SDLPtr.h"

struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace solitaire::graphics {

class SDLWrapper;
struct TextureArea;
struct TextureId;
struct TexturePosition;

class GraphicsSystem {
public:
    GraphicsSystem(std::unique_ptr<SDLWrapper>);
    ~GraphicsSystem();

    void createWindow(const std::string& title,
                      const unsigned width, const unsigned height);

    TextureId loadTexture(const std::string& path);

    void setTextureAlpha(const TextureId, const uint8_t alpha);
    void renderTexture(const TextureId, const TexturePosition&, const TextureArea&);
    void renderTextureOnFullscreen(const TextureId);
    void renderFrame();

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
