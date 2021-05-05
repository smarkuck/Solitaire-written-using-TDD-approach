#pragma once

#include <ostream>

namespace solitaire::geometry {

struct Position {
    int x, y;
};

inline bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline Position operator-(const Position& lhs, const Position& rhs) {
    return Position {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Position operator+(const Position& lhs, const Position& rhs) {
    return Position {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline std::ostream& operator<<(std::ostream& os, const Position& position)
{
    return os << "Position {x: " << position.x << ", y: " << position.y << '}';
}

}
