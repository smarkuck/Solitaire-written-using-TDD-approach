#pragma once

#include "geometry/Position.h"

namespace solitaire::events {

struct NoEvents {};

struct MouseLeftButtonDown {
    geometry::Position position;
};

struct MouseLeftButtonUp {};

struct MouseMove {
    geometry::Position position;
};

struct Quit {};

inline bool operator==(const NoEvents&, const NoEvents&) {
    return true;
}

inline bool operator==(const MouseLeftButtonDown& lhs, const MouseLeftButtonDown& rhs) {
    return lhs.position == rhs.position;
}

inline bool operator==(const MouseLeftButtonUp& lhs, const MouseLeftButtonUp& rhs) {
    return true;
}

inline bool operator==(const MouseMove& lhs, const MouseMove& rhs) {
    return lhs.position == rhs.position;
}

inline bool operator==(const Quit&, const Quit&) {
    return true;
}

}
