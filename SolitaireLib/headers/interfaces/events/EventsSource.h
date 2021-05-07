#pragma once

#include "events/Event.h"

namespace solitaire::events::interfaces {

class EventsSource {
public:
    virtual ~EventsSource() = default;
    virtual Event getEvent() const = 0;
};

}
