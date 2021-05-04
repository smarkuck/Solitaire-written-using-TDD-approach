#include "SDL.h"
#include "events/EventsDefinitions.h"
#include "events/SDLEventsSource.h"
#include "SDL/Wrapper.h"

namespace solitaire::events {

SDLEventsSource::SDLEventsSource(std::unique_ptr<SDL::Wrapper> SDL):
    SDL {std::move(SDL)} {
}

Event SDLEventsSource::getEvent() const {
    SDL_Event event;

    while (SDL->pollEvent(event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                return MouseLeftButtonDown {event.button.x, event.button.y};
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                return MouseLeftButtonUp {event.button.x, event.button.y};
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
