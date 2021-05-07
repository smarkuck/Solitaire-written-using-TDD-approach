#pragma once

#include <memory>

namespace solitaire::archivers::interfaces {

class Snapshot;

class MoveCardsOperationSnapshotCreator {
public:
    virtual ~MoveCardsOperationSnapshotCreator() = default;

    virtual std::unique_ptr<Snapshot>
    createSnapshotIfCardsMovedToOtherPile(
        std::unique_ptr<Snapshot> destinationPileSnapshot) = 0;

    virtual void saveSourcePileSnapshot(std::unique_ptr<Snapshot>) = 0;
    virtual void restoreSourcePile() = 0;
};

}
