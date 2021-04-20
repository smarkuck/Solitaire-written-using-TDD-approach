#pragma once

#include "MoveCardsOperationSnapshotCreator.h"
#include "Snapshot.h"

namespace solitaire::archivers {

class DefaultMoveCardsOperationSnapshotCreator:
    public MoveCardsOperationSnapshotCreator
{
private:
    class Snapshot;

public:
    std::optional<std::unique_ptr<archivers::Snapshot>>
    createSnapshotIfCardsMovedToOtherPile(
        std::unique_ptr<archivers::Snapshot> destinationPileSnapshot) override;

    void saveSourcePileSnapshot(std::unique_ptr<archivers::Snapshot>) override;
    void restoreSourcePile() override;

private:
    void throwIfSourcePileSnapshotIsNullptr() const;
    void throwIfSourcePileSnapshotIsNotNullptr() const;
    void throwIfSnapshotIsNullptr(const std::unique_ptr<archivers::Snapshot>&) const;

    std::unique_ptr<archivers::Snapshot> sourcePileSnapshot;
};

class DefaultMoveCardsOperationSnapshotCreator::Snapshot: public archivers::Snapshot {
public:
    Snapshot(std::unique_ptr<archivers::Snapshot> sourcePileSnapshot,
             std::unique_ptr<archivers::Snapshot> destinationPileSnapshot);

    void restore() const override;
    bool isSnapshotOfSameObject(const archivers::Snapshot&) const override;

private:
    std::unique_ptr<archivers::Snapshot> sourcePileSnapshot;
    std::unique_ptr<archivers::Snapshot> destinationPileSnapshot;
};

}
