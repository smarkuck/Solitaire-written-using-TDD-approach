#include "graphics/DefaultSDLWrapper.h"
#include "graphics/SDLDeleter.h"

namespace solitaire::graphics {

int DefaultSDLWrapper::init(Uint32 flags) const {
    return SDL_Init(flags);
}

void DefaultSDLWrapper::quit() const {
    SDL_Quit();
}

SDLPtr<SDL_Window> DefaultSDLWrapper::createWindow(
    const std::string& title, int x, int y, int w, int h, Uint32 flags) const
{
    return SDLPtr<SDL_Window> {
        SDL_CreateWindow(title.c_str(), x, y, w, h, flags), SDLDeleter {}};
}

SDLPtr<SDL_Renderer> DefaultSDLWrapper::createRenderer(const SDLPtr<SDL_Window>& window,
                                                       int index, Uint32 flags) const
{
    return SDLPtr<SDL_Renderer> {
        SDL_CreateRenderer(window.get(), index, flags), SDLDeleter {}};
}

SDLPtr<SDL_Texture> DefaultSDLWrapper::createTextureFromSurface(
    const SDLPtr<SDL_Renderer>& renderer, const SDLPtr<SDL_Surface>& surface) const
{
    return SDLPtr<SDL_Texture> {
        SDL_CreateTextureFromSurface(renderer.get(), surface.get()), SDLDeleter {}};
}

SDLPtr<SDL_Surface> DefaultSDLWrapper::loadBMP(const std::string& file) const {
    return SDLPtr<SDL_Surface> {SDL_LoadBMP(file.c_str()), SDLDeleter {}};
}

int DefaultSDLWrapper::setTextureAlphaMod(
    const SDLPtr<SDL_Texture>& texture, Uint8 alpha) const
{
    return SDL_SetTextureAlphaMod(texture.get(), alpha);
}

int DefaultSDLWrapper::setColorKey(const SDLPtr<SDL_Surface>& surface,
                                   int flag, Uint32 key) const
{
    return SDL_SetColorKey(surface.get(), flag, key);
}

Uint32 DefaultSDLWrapper::mapRGB(const SDLPtr<SDL_Surface>& surface,
                                 Uint8 r, Uint8 g, Uint8 b) const
{
    return SDL_MapRGB(surface->format, r, g, b);
}

int DefaultSDLWrapper::setRenderDrawColor(const SDLPtr<SDL_Renderer>& renderer,
                                          Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    return SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
}

int DefaultSDLWrapper::renderClear(const SDLPtr<SDL_Renderer>& renderer) const {
    return SDL_RenderClear(renderer.get());
}

int DefaultSDLWrapper::renderCopy(const SDLPtr<SDL_Renderer>& renderer,
                                  const SDLPtr<SDL_Texture>& texture,
                                  const std::optional<SDL_Rect>& srcrect,
                                  const std::optional<SDL_Rect>& dstrect) const
{
    const SDL_Rect* src = srcrect ? &srcrect.value() : nullptr;
    const SDL_Rect* dst = dstrect ? &dstrect.value() : nullptr;
    return SDL_RenderCopy(renderer.get(), texture.get(), src, dst);
}

void DefaultSDLWrapper::renderPresent(const SDLPtr<SDL_Renderer>& renderer) const {
    SDL_RenderPresent(renderer.get());
}

void DefaultSDLWrapper::delay(Uint32 ms) const {
    SDL_Delay(ms);
}

}
