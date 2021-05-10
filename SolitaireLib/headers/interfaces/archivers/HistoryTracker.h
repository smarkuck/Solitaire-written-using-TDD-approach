#pragma once

#include <memory>

namespace solitaire::archivers::interfaces {

class Snapshot;

class HistoryTracker {
public:
    virtual ~HistoryTracker() = default;
    virtual void reset() = 0;
    virtual void save(std::unique_ptr<Snapshot>) = 0;
    virtual void undo() = 0;
    virtual unsigned getHistorySize() const = 0;
};

}
