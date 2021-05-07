#pragma once

#include <memory>
#include "interfaces/events/EventsSource.h"

namespace solitaire {

namespace SDL::interfaces {
class Wrapper;
}

namespace events {

class SDLEventsSource: public interfaces::EventsSource {
public:
    SDLEventsSource(std::unique_ptr<SDL::interfaces::Wrapper>);

    Event getEvent() const override;

private:
    std::unique_ptr<SDL::interfaces::Wrapper> sdl;
};

}
}
