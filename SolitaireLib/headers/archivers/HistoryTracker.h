#pragma once

#include <vector>
#include "interfaces/archivers/HistoryTracker.h"

namespace solitaire::archivers {

class HistoryTracker: public interfaces::HistoryTracker {
public:
    HistoryTracker(const unsigned historyMaxSize = 10);

    void reset() override;
    void save(std::unique_ptr<interfaces::Snapshot>) override;
    void undo() override;

    unsigned getHistorySize() const override;

private:
    const unsigned historyMaxSize;
    std::vector<std::unique_ptr<interfaces::Snapshot>> history;
};

}
