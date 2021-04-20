#pragma once

#include <memory>
#include <optional>

namespace solitaire::archivers {

class Snapshot;

class MoveCardsOperationSnapshotCreator {
public:
    virtual std::optional<std::unique_ptr<archivers::Snapshot>>
    createSnapshotIfCardsMovedToOtherPile(
        std::unique_ptr<archivers::Snapshot> destinationPileSnapshot) = 0;

    virtual void saveSourcePileSnapshot(std::unique_ptr<archivers::Snapshot>) = 0;
    virtual void restoreSourcePile() = 0;
};

}
