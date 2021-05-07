#pragma once

#include "gmock/gmock.h"
#include "interfaces/events/EventsSource.h"

namespace solitaire::events {

class EventsSourceMock: public interfaces::EventsSource {
public:
    MOCK_METHOD(Event, getEvent, (), (const, override));
};

}
