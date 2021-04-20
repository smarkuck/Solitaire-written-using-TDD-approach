#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "archivers/SnapshotMock.h"
#include "gmock/gmock.h"
#include "mock_ptr.h"

using namespace testing;

namespace solitaire::archivers {

class DefaultMoveCardsOperationSnapshotCreatorTests: public Test {
public:
    DefaultMoveCardsOperationSnapshotCreator snapshotCreator;
};

TEST_F(DefaultMoveCardsOperationSnapshotCreatorTests,
       createShapshotShouldThrowIfSourcePileSnapshotNotSaved)
{
    mock_ptr<SnapshotMock> snapshotMock;

    EXPECT_THROW(
        snapshotCreator.createSnapshotIfCardsMovedToOtherPile(snapshotMock.make_unique()),
        std::runtime_error
    );
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorTests,
       saveSourcePileShapshotShouldThrowIfPassedSnapshotIsNullptr)
{
    EXPECT_THROW(snapshotCreator.saveSourcePileSnapshot(nullptr), std::runtime_error);
}

class DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest:
    public DefaultMoveCardsOperationSnapshotCreatorTests
{
public:
    DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest() {
        snapshotCreator.saveSourcePileSnapshot(
            snapshotMock.make_unique()
        );

        EXPECT_CALL(*snapshotMock, isSnapshotOfSameObject(Ref(*snapshotMock2)))
            .WillRepeatedly(Return(false));
    }

    mock_ptr<SnapshotMock> snapshotMock;
    mock_ptr<SnapshotMock> snapshotMock2;
};

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       savingSourcePileSnapshotSecondTimeBeforeUsageShouldThrow)
{
    EXPECT_THROW(
        snapshotCreator.saveSourcePileSnapshot(snapshotMock2.make_unique()),
        std::runtime_error
    );
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       createSnapshotShouldThrowIfPassedSnapshotIsNullptr)
{
    EXPECT_THROW(
        snapshotCreator.createSnapshotIfCardsMovedToOtherPile(nullptr),
        std::runtime_error
    );
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       ifPileSnapshotsAreFromSameObjectDontCreateSnapshot)
{
    EXPECT_CALL(*snapshotMock, isSnapshotOfSameObject(Ref(*snapshotMock2)))
        .WillOnce(Return(true));

    EXPECT_EQ(
        snapshotCreator.createSnapshotIfCardsMovedToOtherPile(snapshotMock2.make_unique()),
        std::nullopt
    );
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest, createSnapshot) {
    auto snapshot = snapshotCreator.createSnapshotIfCardsMovedToOtherPile(
        snapshotMock2.make_unique()
    );

    EXPECT_CALL(*snapshotMock, restore());
    EXPECT_CALL(*snapshotMock2, restore());
    snapshot.value()->restore();
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       isSnapshotOfSameObjectAlwaysReturnsFalse)
{
    auto snapshot = snapshotCreator.createSnapshotIfCardsMovedToOtherPile(
        snapshotMock2.make_unique()
    );

    EXPECT_FALSE(snapshot.value()->isSnapshotOfSameObject(*snapshot.value()));
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       restoreSourcePileShouldThrowIfSnapshotCreated)
{
    snapshotCreator.createSnapshotIfCardsMovedToOtherPile(snapshotMock2.make_unique());
    EXPECT_THROW(snapshotCreator.restoreSourcePile(), std::runtime_error);
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest,
       restoreSourcePileIfSnapshotNotCreated)
{
    EXPECT_CALL(*snapshotMock, isSnapshotOfSameObject(Ref(*snapshotMock2)))
        .WillOnce(Return(true));

    snapshotCreator.createSnapshotIfCardsMovedToOtherPile(snapshotMock2.make_unique());
    EXPECT_CALL(*snapshotMock, restore());
    snapshotCreator.restoreSourcePile();
}

class DefaultMoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest:
    public DefaultMoveCardsOperationSnapshotCreatorWithSavedSourcePileTest
{
public:
    DefaultMoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest() {
        EXPECT_CALL(*snapshotMock, restore());
        snapshotCreator.restoreSourcePile();
    }
};

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest, restoreSourcePile)
{
}

TEST_F(DefaultMoveCardsOperationSnapshotCreatorWithRestoredSourcePileTest,
       restoreSourcePileShouldThrowIfAlreadyRestored)
{
    EXPECT_THROW(snapshotCreator.restoreSourcePile(), std::runtime_error);
}

}
