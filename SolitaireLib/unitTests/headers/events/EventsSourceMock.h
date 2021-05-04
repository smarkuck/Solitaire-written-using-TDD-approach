#pragma once

#include "events/EventsSource.h"
#include "gmock/gmock.h"

namespace solitaire::events {

class EventsSourceMock: public EventsSource {
public:
    MOCK_METHOD(Event, getEvent, (), (const, override));
};

}
