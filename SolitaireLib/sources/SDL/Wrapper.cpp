#include "SDL/PtrDeleter.h"
#include "SDL/Wrapper.h"

namespace solitaire::SDL {

int Wrapper::init(Uint32 flags) const {
    return SDL_Init(flags);
}

void Wrapper::quit() const {
    SDL_Quit();
}

int Wrapper::pollEvent(SDL_Event& event) const {
    return SDL_PollEvent(&event);
}

UniquePtr<SDL_Window> Wrapper::createWindow(
    const std::string& title, int x, int y, int w, int h, Uint32 flags) const
{
    return UniquePtr<SDL_Window> {
        SDL_CreateWindow(title.c_str(), x, y, w, h, flags), PtrDeleter {}};
}

UniquePtr<SDL_Renderer> Wrapper::createRenderer(const UniquePtr<SDL_Window>& window,
                                                int index, Uint32 flags) const
{
    return UniquePtr<SDL_Renderer> {
        SDL_CreateRenderer(window.get(), index, flags), PtrDeleter {}};
}

UniquePtr<SDL_Texture> Wrapper::createTextureFromSurface(
    const UniquePtr<SDL_Renderer>& renderer, const UniquePtr<SDL_Surface>& surface) const
{
    return UniquePtr<SDL_Texture> {
        SDL_CreateTextureFromSurface(renderer.get(), surface.get()), PtrDeleter {}};
}

UniquePtr<SDL_Surface> Wrapper::loadBMP(const std::string& file) const {
    return UniquePtr<SDL_Surface> {SDL_LoadBMP(file.c_str()), PtrDeleter {}};
}

int Wrapper::setTextureAlphaMod(const UniquePtr<SDL_Texture>& texture, Uint8 alpha) const {
    return SDL_SetTextureAlphaMod(texture.get(), alpha);
}

int Wrapper::setColorKey(const UniquePtr<SDL_Surface>& surface,
                         int flag, Uint32 key) const
{
    return SDL_SetColorKey(surface.get(), flag, key);
}

Uint32 Wrapper::mapRGB(const UniquePtr<SDL_Surface>& surface,
                       Uint8 r, Uint8 g, Uint8 b) const
{
    return SDL_MapRGB(surface->format, r, g, b);
}

int Wrapper::setRenderDrawColor(const UniquePtr<SDL_Renderer>& renderer,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    return SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
}

int Wrapper::renderClear(const UniquePtr<SDL_Renderer>& renderer) const {
    return SDL_RenderClear(renderer.get());
}

int Wrapper::renderCopy(const UniquePtr<SDL_Renderer>& renderer,
                        const UniquePtr<SDL_Texture>& texture,
                        const std::optional<SDL_Rect>& srcrect,
                        const std::optional<SDL_Rect>& dstrect) const
{
    const SDL_Rect* src = srcrect ? &srcrect.value() : nullptr;
    const SDL_Rect* dst = dstrect ? &dstrect.value() : nullptr;
    return SDL_RenderCopy(renderer.get(), texture.get(), src, dst);
}

void Wrapper::renderPresent(const UniquePtr<SDL_Renderer>& renderer) const {
    SDL_RenderPresent(renderer.get());
}

void Wrapper::delay(Uint32 ms) const {
    SDL_Delay(ms);
}

}
