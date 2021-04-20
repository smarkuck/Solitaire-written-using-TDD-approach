#pragma once

#include "archivers/Snapshot.h"
#include "gmock/gmock.h"

namespace solitaire::archivers {

class SnapshotMock: public Snapshot {
public:
    MOCK_METHOD(void, restore, (), (const, override));
    MOCK_METHOD(bool, isSnapshotOfSameObject, (const Snapshot&), (const, override));
};

}
