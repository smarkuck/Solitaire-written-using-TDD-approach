#include "SDL.h"
#include "events/EventsDefinitions.h"
#include "events/SDLEventsSource.h"
#include "geometry/Position.h"
#include "interfaces/SDL/Wrapper.h"

using namespace solitaire::geometry;

namespace solitaire::events {

SDLEventsSource::SDLEventsSource(std::unique_ptr<SDL::interfaces::Wrapper> sdl):
    sdl {std::move(sdl)} {
}

Event SDLEventsSource::getEvent() const {
    SDL_Event event;

    while (sdl->pollEvent(event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                return MouseLeftButtonDown {Position {event.button.x, event.button.y}};
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                return MouseLeftButtonUp {};
            break;

        case SDL_MOUSEMOTION:
            return MouseMove {event.motion.x, event.motion.y};

        case SDL_QUIT:
            return Quit {};
        }
    }

    return NoEvents {};
}

}
