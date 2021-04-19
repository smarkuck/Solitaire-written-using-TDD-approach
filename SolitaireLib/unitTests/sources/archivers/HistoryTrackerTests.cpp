#include "archivers/HistoryTracker.h"
#include "archivers/SnapshotMock.h"
#include "gmock/gmock.h"
#include "mock_ptr.h"

using namespace testing;

namespace solitaire::archivers {

namespace {
constexpr unsigned maxHistorySize {2};
}

class HistoryTrackerTests: public Test {
public:
    HistoryTracker historyTracker {maxHistorySize};
};

TEST_F(HistoryTrackerTests, historyIsEmpty) {
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

TEST_F(HistoryTrackerTests, undoShouldThrowIfHistoryEmpty) {
    EXPECT_THROW(historyTracker.undo(), std::runtime_error);
}

TEST_F(HistoryTrackerTests, onSaveHistorySizeShouldIncrease) {
    mock_ptr<SnapshotMock> snapshotMock;
    historyTracker.save(snapshotMock.make_unique());
    EXPECT_EQ(historyTracker.getHistorySize(), 1);
}

class HistoryTrackerWithOneSnapshotTests: public HistoryTrackerTests {
public:
    HistoryTrackerWithOneSnapshotTests() {
        historyTracker.save(snapshotMock.make_unique());
    }

    mock_ptr<StrictMock<SnapshotMock>> snapshotMock;
};

TEST_F(HistoryTrackerWithOneSnapshotTests, onUndoRestoreAndRemoveSnapshot) {
    EXPECT_CALL(*snapshotMock, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

class FullHistoryTrackerTests: public HistoryTrackerWithOneSnapshotTests {
public:
    FullHistoryTrackerTests() {
        historyTracker.save(snapshotMock2.make_unique());
    }

    mock_ptr<SnapshotMock> snapshotMock2;
};

TEST_F(FullHistoryTrackerTests, onUndoRestoreSnapshotsInReverseOrder) {
    EXPECT_CALL(*snapshotMock2, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 1);

    EXPECT_CALL(*snapshotMock, restore());
    historyTracker.undo();
    EXPECT_EQ(historyTracker.getHistorySize(), 0);
}

TEST_F(FullHistoryTrackerTests, removeFirstSnapshotOnSaveWhenHistoryFull) {
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
