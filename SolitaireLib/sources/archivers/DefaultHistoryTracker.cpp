#include <stdexcept>

#include "archivers/DefaultHistoryTracker.h"
#include "archivers/Snapshot.h"

namespace solitaire::archivers {

DefaultHistoryTracker::DefaultHistoryTracker(const unsigned historyMaxSize):
    historyMaxSize {historyMaxSize} {
}

void DefaultHistoryTracker::save(std::unique_ptr<Snapshot> snapshot) {
    if (not snapshot)
        throw std::runtime_error {"Passed snapshot is nullptr"};

    if (history.size() >= historyMaxSize)
        history.erase(history.begin());
    history.emplace_back(std::move(snapshot));
}

void DefaultHistoryTracker::undo() {
    if (history.empty())
        throw std::runtime_error {"Cannot undo operation when history is empty."};

    history.back()->restore();
    history.pop_back();
}

unsigned DefaultHistoryTracker::getHistorySize() const {
    return history.size();
}

}
