#pragma once

namespace solitaire::events {

struct NoEvents {};

struct MouseLeftButtonDown {
    int x, y;
};

struct MouseLeftButtonUp {
    int x, y;
};

struct MouseMove {
    int x, y;
};

struct Quit {};

inline bool operator==(const NoEvents&, const NoEvents&) {
    return true;
}

inline bool operator==(const MouseLeftButtonDown& lhs, const MouseLeftButtonDown& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline bool operator==(const MouseLeftButtonUp& lhs, const MouseLeftButtonUp& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline bool operator==(const MouseMove& lhs, const MouseMove& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline bool operator==(const Quit&, const Quit&) {
    return true;
}

}
