#pragma once

#include "events/EventsProcessor.h"
#include "gmock/gmock.h"

namespace solitaire::events {

class EventsProcessorMock: public EventsProcessor {
public:
    MOCK_METHOD(void, processEvents, (), (override));
    MOCK_METHOD(bool, shouldQuit, (), (const, override));
};

}
