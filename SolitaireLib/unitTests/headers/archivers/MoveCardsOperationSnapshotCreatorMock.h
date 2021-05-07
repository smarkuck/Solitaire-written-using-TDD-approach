#pragma once

#include "gmock/gmock.h"
#include "interfaces/archivers/MoveCardsOperationSnapshotCreator.h"

namespace solitaire::archivers {

class MoveCardsOperationSnapshotCreatorMock:
    public interfaces::MoveCardsOperationSnapshotCreator
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
