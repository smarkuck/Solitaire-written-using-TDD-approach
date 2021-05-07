#pragma once

#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "gmock/gmock.h"

namespace solitaire::archivers {

class MoveCardsOperationSnapshotCreatorMock:
    public MoveCardsOperationSnapshotCreator
{
public:
    MOCK_METHOD(std::unique_ptr<interfaces::Snapshot>,
                createSnapshotIfCardsMovedToOtherPile,
                (std::unique_ptr<interfaces::Snapshot>), (override));

    MOCK_METHOD(void, saveSourcePileSnapshot,
                (std::unique_ptr<interfaces::Snapshot>), (override));
    MOCK_METHOD(void, restoreSourcePile, (), (override));
};

}
