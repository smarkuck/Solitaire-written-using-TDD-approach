#pragma once

#include <memory>
#include <string>

#include "SDLPtr.h"

struct SDL_Window;
struct SDL_Renderer;

namespace solitaire::graphics {

class SDLWrapper;

class GraphicsSystem {
public:
    GraphicsSystem(std::unique_ptr<SDLWrapper>);
    ~GraphicsSystem();

    void createWindow(const std::string& title, unsigned width, unsigned height);

private:
    void initializeSDLOrQuitAndThrowError();

    SDLPtr<SDL_Window> createSDLWindowOrQuitAndThrowError(
        const std::string& title, unsigned width, unsigned height);

    SDLPtr<SDL_Renderer>
    createSDLWindowRendererOrQuitAndThrowError(const SDLPtr<SDL_Window>&);

    void quitAndThrow(const std::string& error);
    void quit();

    std::unique_ptr<SDLWrapper> SDL;
    SDLPtr<SDL_Window> window;
    SDLPtr<SDL_Renderer> renderer;

    bool isSDLInitialized {false};
    bool isWindowCreated {false};
};

}
