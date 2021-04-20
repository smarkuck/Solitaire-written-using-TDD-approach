#pragma once

#include "archivers/HistoryTracker.h"
#include "gmock/gmock.h"

namespace solitaire::archivers {

class HistoryTrackerMock: public HistoryTracker {
public:
    MOCK_METHOD(void, save, (std::unique_ptr<Snapshot>), (override));
    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(unsigned, getHistorySize, (), (const, override));
};

}
