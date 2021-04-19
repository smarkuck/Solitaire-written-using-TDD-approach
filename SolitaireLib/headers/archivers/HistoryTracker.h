#pragma once

#include <memory>
#include <vector>

namespace solitaire::archivers {

class Snapshot;

class HistoryTracker {
public:
    HistoryTracker(unsigned historyMaxSize = 10);

    void save(std::unique_ptr<Snapshot>);
    void undo();

    unsigned getHistorySize() const;

private:
    const unsigned historyMaxSize;
    std::vector<std::unique_ptr<Snapshot>> history;
};

}
