#include <stdexcept>

#include "archivers/HistoryTracker.h"
#include "archivers/Snapshot.h"

namespace solitaire::archivers {

HistoryTracker::HistoryTracker(unsigned historyMaxSize):
    historyMaxSize {historyMaxSize} {
}

void HistoryTracker::save(std::unique_ptr<Snapshot> snapshot) {
    if (not snapshot)
        throw std::runtime_error {"Passed snapshot is nullptr"};

    if (history.size() >= historyMaxSize)
        history.erase(history.begin());
    history.emplace_back(std::move(snapshot));
}

void HistoryTracker::undo() {
    if (history.empty())
        throw std::runtime_error {"Cannot undo operation when history is empty."};

    history.back()->restore();
    history.pop_back();
}

unsigned HistoryTracker::getHistorySize() const {
    return history.size();
}

}
