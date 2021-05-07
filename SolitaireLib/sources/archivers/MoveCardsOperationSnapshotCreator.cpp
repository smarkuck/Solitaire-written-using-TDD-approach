#include <stdexcept>
#include "archivers/MoveCardsOperationSnapshotCreator.h"

namespace solitaire::archivers {

std::unique_ptr<interfaces::Snapshot>
MoveCardsOperationSnapshotCreator::createSnapshotIfCardsMovedToOtherPile(
    std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot)
{
    throwIfSourcePileSnapshotIsNullptr();
    throwIfSnapshotIsNullptr(destinationPileSnapshot);

    if (sourcePileSnapshot->isSnapshotOfSameObject(*destinationPileSnapshot))
    {
        sourcePileSnapshot = nullptr;
        return nullptr;
    }

    return std::make_unique<Snapshot>(
        std::move(sourcePileSnapshot), std::move(destinationPileSnapshot)
    );
}

void MoveCardsOperationSnapshotCreator::saveSourcePileSnapshot(
    std::unique_ptr<interfaces::Snapshot> snapshot)
{
    throwIfSourcePileSnapshotIsNotNullptr();
    throwIfSnapshotIsNullptr(snapshot);
    sourcePileSnapshot = std::move(snapshot);
}

void MoveCardsOperationSnapshotCreator::restoreSourcePile() {
    throwIfSourcePileSnapshotIsNullptr();
    sourcePileSnapshot->restore();
    sourcePileSnapshot = nullptr;
}

void MoveCardsOperationSnapshotCreator::throwIfSourcePileSnapshotIsNullptr() const {
    if (not sourcePileSnapshot)
        throw std::runtime_error {"Source pile snapshot is nullptr."};
}

void MoveCardsOperationSnapshotCreator::throwIfSourcePileSnapshotIsNotNullptr() const {
    if (sourcePileSnapshot)
        throw std::runtime_error {"Source pile snapshot is already saved."};
}

void MoveCardsOperationSnapshotCreator::throwIfSnapshotIsNullptr(
    const std::unique_ptr<interfaces::Snapshot>& snapshot) const
{
    if (not snapshot)
        throw std::runtime_error {"Passed snapshot is nullptr."};
}

MoveCardsOperationSnapshotCreator::Snapshot::Snapshot(
    std::unique_ptr<interfaces::Snapshot> sourcePileSnapshot,
    std::unique_ptr<interfaces::Snapshot> destinationPileSnapshot):
        sourcePileSnapshot {std::move(sourcePileSnapshot)},
        destinationPileSnapshot {std::move(destinationPileSnapshot)} {
}

void MoveCardsOperationSnapshotCreator::Snapshot::restore() const {
    sourcePileSnapshot->restore();
    destinationPileSnapshot->restore();
}

bool MoveCardsOperationSnapshotCreator::Snapshot::isSnapshotOfSameObject(
    const interfaces::Snapshot&) const
{
    return false;
}

}
