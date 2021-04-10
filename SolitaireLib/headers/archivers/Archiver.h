#pragma once

#include <memory>

namespace solitaire::archivers {

class Snapshot;

class Archiver {
public:
    virtual ~Archiver() = default;
    virtual std::unique_ptr<Snapshot> createSnapshot() = 0;
};

}
