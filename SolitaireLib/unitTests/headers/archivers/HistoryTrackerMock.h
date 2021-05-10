#pragma once

#include "gmock/gmock.h"
#include "interfaces/archivers/HistoryTracker.h"

namespace solitaire::archivers {

class HistoryTrackerMock: public interfaces::HistoryTracker {
public:
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, save, (std::unique_ptr<interfaces::Snapshot>), (override));
    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(unsigned, getHistorySize, (), (const, override));
};

}
