#pragma once

#include <memory>
#include "archivers/Snapshot.h"

namespace solitaire::archivers {

class Snapshot;

class MoveCardsOperationSnapshotCreator {
private:
    class Snapshot;

public:
    std::unique_ptr<archivers::Snapshot> createSnapshot(
        std::unique_ptr<archivers::Snapshot> destinationPileSnapshot);

    void saveSourcePileSnapshot(std::unique_ptr<archivers::Snapshot>);
    void restoreSourcePile();

private:
    void throwIfSourcePileSnapshotIsNullptr() const;
    void throwIfSourcePileSnapshotIsNotNullptr() const;
    void throwIfSnapshotIsNullptr(const std::unique_ptr<archivers::Snapshot>&) const;

    std::unique_ptr<archivers::Snapshot> sourcePileSnapshot;
};

class MoveCardsOperationSnapshotCreator::Snapshot: public archivers::Snapshot {
public:
    Snapshot(std::unique_ptr<archivers::Snapshot> sourcePileSnapshot,
                std::unique_ptr<archivers::Snapshot> destinationPileSnapshot);

    void restore() const override;

private:
    std::unique_ptr<archivers::Snapshot> sourcePileSnapshot;
    std::unique_ptr<archivers::Snapshot> destinationPileSnapshot;
};

}
