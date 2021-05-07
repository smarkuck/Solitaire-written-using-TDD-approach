#pragma once

#include <memory>
#include "interfaces/events/EventsSource.h"

namespace solitaire {

namespace SDL {
class Wrapper;
}

namespace events {

class SDLEventsSource: public interfaces::EventsSource {
public:
    SDLEventsSource(std::unique_ptr<SDL::Wrapper>);

    Event getEvent() const override;

private:
    std::unique_ptr<SDL::Wrapper> sdl;
};

}
}
