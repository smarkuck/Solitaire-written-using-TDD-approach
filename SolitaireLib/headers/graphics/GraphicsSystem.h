#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SDLPtr.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

namespace solitaire::graphics {

class SDLWrapper;
struct TextureId;

class GraphicsSystem {
public:
    GraphicsSystem(std::unique_ptr<SDLWrapper>);
    ~GraphicsSystem();

    void createWindow(const std::string& title, unsigned width, unsigned height);
    TextureId loadTexture(const std::string& path, unsigned width, unsigned height);
    void setTextureAlpha(const TextureId, uint8_t alpha);

private:
    struct TextureData {
        SDLPtr<SDL_Texture> texture;
        unsigned width, height;
    };

    void initializeSDLOrQuitAndThrowError();

    SDLPtr<SDL_Window> createSDLWindowOrQuitAndThrowError(
        const std::string& title, unsigned width, unsigned height);

    SDLPtr<SDL_Renderer>
    createSDLWindowRendererOrQuitAndThrowError(const SDLPtr<SDL_Window>&);

    SDLPtr<SDL_Surface> createSDLSurfaceOrThrow(const std::string& path) const;
    SDLPtr<SDL_Texture> createSDLTextureOrThrow(const SDLPtr<SDL_Surface>&) const;

    void quitAndThrow(const std::string& error);
    void quit();

    std::unique_ptr<SDLWrapper> SDL;
    SDLPtr<SDL_Window> window;
    SDLPtr<SDL_Renderer> renderer;
    std::vector<TextureData> textures;

    bool isSDLInitialized {false};
    bool isWindowCreated {false};
};

}
