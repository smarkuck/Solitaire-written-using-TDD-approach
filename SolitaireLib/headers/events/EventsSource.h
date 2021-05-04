#pragma once

#include "Event.h"

namespace solitaire::events {

class EventsSource {
public:
    virtual ~EventsSource() = default;
    virtual Event getEvent() const = 0;
};

}
