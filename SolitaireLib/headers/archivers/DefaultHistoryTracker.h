#pragma once

#include <vector>
#include "HistoryTracker.h"

namespace solitaire::archivers {

class DefaultHistoryTracker: public HistoryTracker {
public:
    DefaultHistoryTracker(const unsigned historyMaxSize = 10);

    void save(std::unique_ptr<interfaces::Snapshot>) override;
    void undo() override;

    unsigned getHistorySize() const override;

private:
    const unsigned historyMaxSize;
    std::vector<std::unique_ptr<interfaces::Snapshot>> history;
};

}
