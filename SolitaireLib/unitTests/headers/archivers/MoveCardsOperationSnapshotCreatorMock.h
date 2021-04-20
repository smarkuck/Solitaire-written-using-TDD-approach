#pragma once

#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "gmock/gmock.h"

namespace solitaire::archivers {

class MoveCardsOperationSnapshotCreatorMock: public MoveCardsOperationSnapshotCreator {
public:
    MOCK_METHOD(std::optional<std::unique_ptr<archivers::Snapshot>>,
                createSnapshotIfCardsMovedToOtherPile,
                (std::unique_ptr<archivers::Snapshot>), (override));

    MOCK_METHOD(void, saveSourcePileSnapshot, (std::unique_ptr<archivers::Snapshot>), (override));
    MOCK_METHOD(void, restoreSourcePile, (), (override));
};

}
