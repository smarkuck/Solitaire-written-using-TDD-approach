#include "SDL/DefaultWrapper.h"
#include "SDL/PtrDeleter.h"

namespace solitaire::SDL {

int DefaultWrapper::init(Uint32 flags) const {
    return SDL_Init(flags);
}

void DefaultWrapper::quit() const {
    SDL_Quit();
}

UniquePtr<SDL_Window> DefaultWrapper::createWindow(
    const std::string& title, int x, int y, int w, int h, Uint32 flags) const
{
    return UniquePtr<SDL_Window> {
        SDL_CreateWindow(title.c_str(), x, y, w, h, flags), PtrDeleter {}};
}

UniquePtr<SDL_Renderer> DefaultWrapper::createRenderer(const UniquePtr<SDL_Window>& window,
                                                       int index, Uint32 flags) const
{
    return UniquePtr<SDL_Renderer> {
        SDL_CreateRenderer(window.get(), index, flags), PtrDeleter {}};
}

UniquePtr<SDL_Texture> DefaultWrapper::createTextureFromSurface(
    const UniquePtr<SDL_Renderer>& renderer, const UniquePtr<SDL_Surface>& surface) const
{
    return UniquePtr<SDL_Texture> {
        SDL_CreateTextureFromSurface(renderer.get(), surface.get()), PtrDeleter {}};
}

UniquePtr<SDL_Surface> DefaultWrapper::loadBMP(const std::string& file) const {
    return UniquePtr<SDL_Surface> {SDL_LoadBMP(file.c_str()), PtrDeleter {}};
}

int DefaultWrapper::setTextureAlphaMod(
    const UniquePtr<SDL_Texture>& texture, Uint8 alpha) const
{
    return SDL_SetTextureAlphaMod(texture.get(), alpha);
}

int DefaultWrapper::setColorKey(const UniquePtr<SDL_Surface>& surface,
                                int flag, Uint32 key) const
{
    return SDL_SetColorKey(surface.get(), flag, key);
}

Uint32 DefaultWrapper::mapRGB(const UniquePtr<SDL_Surface>& surface,
                              Uint8 r, Uint8 g, Uint8 b) const
{
    return SDL_MapRGB(surface->format, r, g, b);
}

int DefaultWrapper::setRenderDrawColor(const UniquePtr<SDL_Renderer>& renderer,
                                       Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    return SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
}

int DefaultWrapper::renderClear(const UniquePtr<SDL_Renderer>& renderer) const {
    return SDL_RenderClear(renderer.get());
}

int DefaultWrapper::renderCopy(const UniquePtr<SDL_Renderer>& renderer,
                               const UniquePtr<SDL_Texture>& texture,
                               const std::optional<SDL_Rect>& srcrect,
                               const std::optional<SDL_Rect>& dstrect) const
{
    const SDL_Rect* src = srcrect ? &srcrect.value() : nullptr;
    const SDL_Rect* dst = dstrect ? &dstrect.value() : nullptr;
    return SDL_RenderCopy(renderer.get(), texture.get(), src, dst);
}

void DefaultWrapper::renderPresent(const UniquePtr<SDL_Renderer>& renderer) const {
    SDL_RenderPresent(renderer.get());
}

void DefaultWrapper::delay(Uint32 ms) const {
    SDL_Delay(ms);
}

}
