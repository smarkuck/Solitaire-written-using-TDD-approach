#pragma once

#include <memory>

namespace solitaire::archivers {

namespace interfaces {
class Snapshot;
}

class MoveCardsOperationSnapshotCreator {
public:
    virtual ~MoveCardsOperationSnapshotCreator() = default;

    virtual std::unique_ptr<interfaces::Snapshot>
    createSnapshotIfCardsMovedToOtherPile(
        std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot) = 0;

    virtual void saveSourcePileSnapshot(std::unique_ptr<interfaces::Snapshot>) = 0;
    virtual void restoreSourcePile() = 0;
};

}
