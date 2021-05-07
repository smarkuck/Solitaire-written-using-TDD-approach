#pragma once

#include "gmock/gmock.h"
#include "interfaces/events/EventsProcessor.h"

namespace solitaire::events {

class EventsProcessorMock: public interfaces::EventsProcessor {
public:
    MOCK_METHOD(void, processEvents, (), (override));
    MOCK_METHOD(bool, shouldQuit, (), (const, override));
};

}
