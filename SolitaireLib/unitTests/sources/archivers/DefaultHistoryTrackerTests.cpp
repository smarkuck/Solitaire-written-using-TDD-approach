#include "archivers/DefaultHistoryTracker.h"
#include "archivers/SnapshotMock.h"
#include "gmock/gmock.h"
#include "mock_ptr.h"

using namespace testing;

namespace solitaire::archivers {

namespace {
constexpr unsigned maxHistorySize {2};
}

class DefaultHistoryTrackerTests: public Test {
public:
    DefaultHistoryTracker historyTracker {maxHistorySize};
};

TEST_F(DefaultHistoryTrackerTests, historyIsEmpty) {
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

TEST_F(DefaultHistoryTrackerTests, undoShouldThrowIfHistoryEmpty) {
    EXPECT_THROW(historyTracker.undo(), std::runtime_error);
}

TEST_F(DefaultHistoryTrackerTests, onSaveShouldThrowIfPassedNullptr) {
    EXPECT_THROW(historyTracker.save(nullptr), std::runtime_error);
}

TEST_F(DefaultHistoryTrackerTests, onSaveHistorySizeShouldIncrease) {
    mock_ptr<SnapshotMock> snapshotMock;
    historyTracker.save(snapshotMock.make_unique());
    EXPECT_EQ(historyTracker.getHistorySize(), 1);
}

class DefaultHistoryTrackerWithOneSnapshotTests: public DefaultHistoryTrackerTests {
public:
    DefaultHistoryTrackerWithOneSnapshotTests() {
        historyTracker.save(snapshotMock.make_unique());
    }

    mock_ptr<StrictMock<SnapshotMock>> snapshotMock;
};

TEST_F(DefaultHistoryTrackerWithOneSnapshotTests, onUndoRestoreAndRemoveSnapshot) {
    EXPECT_CALL(*snapshotMock, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

class FullDefaultHistoryTrackerTests: public DefaultHistoryTrackerWithOneSnapshotTests {
public:
    FullDefaultHistoryTrackerTests() {
        historyTracker.save(snapshotMock2.make_unique());
    }

    mock_ptr<SnapshotMock> snapshotMock2;
};

TEST_F(FullDefaultHistoryTrackerTests, onUndoRestoreSnapshotsInReverseOrder) {
    EXPECT_CALL(*snapshotMock2, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 1);

    EXPECT_CALL(*snapshotMock, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

TEST_F(FullDefaultHistoryTrackerTests, removeFirstSnapshotOnSaveWhenHistoryFull) {
    mock_ptr<SnapshotMock> snapshotMock3;
    historyTracker.save(snapshotMock3.make_unique());

    EXPECT_CALL(*snapshotMock3, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 1);

    EXPECT_CALL(*snapshotMock2, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

}
