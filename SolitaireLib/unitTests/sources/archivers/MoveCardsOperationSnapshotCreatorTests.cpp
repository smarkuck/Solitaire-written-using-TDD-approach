#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "archivers/SnapshotMock.h"
#include "gmock/gmock.h"
#include "mock_ptr.h"

using namespace testing;

namespace solitaire::archivers {

class MoveCardsOperationSnapshotCreatorTests: public Test {
public:
    MoveCardsOperationSnapshotCreator snapshotCreator;
};

TEST_F(MoveCardsOperationSnapshotCreatorTests,
       createShapshotShouldThrowIfSourcePileSnapshotNotSaved)
{
    mock_ptr<SnapshotMock> snapshotMock;

    EXPECT_THROW(
        snapshotCreator.createSnapshot(snapshotMock.make_unique()),
        std::runtime_error
    );
}

TEST_F(MoveCardsOperationSnapshotCreatorTests,
       saveSourcePileShapshotShouldThrowIfPassedSnapshotIsNullptr)
{
    EXPECT_THROW(snapshotCreator.saveSourcePileSnapshot(nullptr), std::runtime_error);
}

class MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest:
    public MoveCardsOperationSnapshotCreatorTests
{
public:
    MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest() {
        snapshotCreator.saveSourcePileSnapshot(
            snapshotMock.make_unique()
        );
    }

    mock_ptr<SnapshotMock> snapshotMock;
    mock_ptr<SnapshotMock> snapshotMock2;
};

TEST_F(MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       savingSourcePileSnapshotSecondTimeBeforeUsageShouldThrow)
{
    EXPECT_THROW(
        snapshotCreator.saveSourcePileSnapshot(snapshotMock2.make_unique()),
        std::runtime_error
    );
}

TEST_F(MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       createSnapshotShouldThrowIfPassedSnapshotIsNullptr)
{
    EXPECT_THROW(snapshotCreator.createSnapshot(nullptr), std::runtime_error);
}

TEST_F(MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest, createSnapshot) {
    auto snapshot = snapshotCreator.createSnapshot(snapshotMock2.make_unique());

    EXPECT_CALL(*snapshotMock, restore());
    EXPECT_CALL(*snapshotMock2, restore());
    snapshot->restore();
}

TEST_F(MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       restoreSourcePileShouldThrowIfSnapshotCreated)
{
    snapshotCreator.createSnapshot(snapshotMock2.make_unique());
    EXPECT_THROW(snapshotCreator.restoreSourcePile(), std::runtime_error);
}

class MoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest:
    public MoveCardsOperationSnapshotCreatorWithSavedSourcePileTest
{
public:
    MoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest() {
        EXPECT_CALL(*snapshotMock, restore());
        snapshotCreator.restoreSourcePile();
    }
};

TEST_F(MoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest, restoreSourcePile)
{
}

TEST_F(MoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest,
       restoreSourcePileShouldThrowIfAlreadyRestored)
{
    EXPECT_THROW(snapshotCreator.restoreSourcePile(), std::runtime_error);
}

}
