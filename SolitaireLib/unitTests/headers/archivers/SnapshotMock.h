#pragma once

#include "gmock/gmock.h"
#include "interfaces/archivers/Snapshot.h"

namespace solitaire::archivers {

class SnapshotMock: public interfaces::Snapshot {
public:
    MOCK_METHOD(void, restore, (), (const, override));
    MOCK_METHOD(bool, isSnapshotOfSameObject,
                (const interfaces::Snapshot&), (const, override));
};

}
