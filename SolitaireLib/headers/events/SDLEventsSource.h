#pragma once

#include <memory>
#include "EventsSource.h"

namespace solitaire {

namespace SDL {
class Wrapper;
}

namespace events {

class SDLEventsSource: public EventsSource {
public:
    SDLEventsSource(std::unique_ptr<SDL::Wrapper>);

    Event getEvent() const override;

private:
    std::unique_ptr<SDL::Wrapper> SDL;
};

}
}
