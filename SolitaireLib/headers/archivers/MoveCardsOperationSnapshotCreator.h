#pragma once

#include "interfaces/archivers/MoveCardsOperationSnapshotCreator.h"
#include "interfaces/archivers/Snapshot.h"

namespace solitaire::archivers {

class MoveCardsOperationSnapshotCreator:
    public interfaces::MoveCardsOperationSnapshotCreator
{
private:
    class Snapshot;

public:
    std::unique_ptr<interfaces::Snapshot> createSnapshotIfCardsMovedToOtherPile(
        std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot) override;

    void saveSourcePileSnapshot(std::unique_ptr<interfaces::Snapshot>) override;
    void restoreSourcePile() override;

private:
    void throwIfSourcePileSnapshotIsNullptr() const;
    void throwIfSourcePileSnapshotIsNotNullptr() const;
    void throwIfSnapshotIsNullptr(const std::unique_ptr<interfaces::Snapshot>&) const;

    std::unique_ptr<interfaces::Snapshot> sourcePileSnapshot;
};

class MoveCardsOperationSnapshotCreator::Snapshot: public interfaces::Snapshot {
public:
    Snapshot(std::unique_ptr<interfaces::Snapshot> sourcePileSnapshot,
             std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot);

    void restore() const override;
    bool isSnapshotOfSameObject(const interfaces::Snapshot&) const override;

private:
    std::unique_ptr<interfaces::Snapshot> sourcePileSnapshot;
    std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot;
};

}
