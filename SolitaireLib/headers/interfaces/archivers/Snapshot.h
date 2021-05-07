#pragma once

namespace solitaire::archivers::interfaces {

class Snapshot {
public:
    virtual ~Snapshot() = default;
    virtual void restore() const = 0;
    virtual bool isSnapshotOfSameObject(const Snapshot&) const = 0;
};

}
