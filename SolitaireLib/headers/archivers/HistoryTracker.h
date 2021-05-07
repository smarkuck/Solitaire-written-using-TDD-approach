#pragma once

#include <memory>

namespace solitaire::archivers {

namespace interfaces {
class Snapshot;
}

class HistoryTracker {
public:
    virtual ~HistoryTracker() = default;
    virtual void save(std::unique_ptr<interfaces::Snapshot>) = 0;
    virtual void undo() = 0;
    virtual unsigned getHistorySize() const = 0;
};

}
