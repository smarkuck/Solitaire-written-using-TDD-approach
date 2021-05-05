#pragma once

#include <ostream>

namespace solitaire::graphics {

struct TexturePosition {
    int x, y;
};

inline bool operator==(const TexturePosition& lhs, const TexturePosition& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline std::ostream& operator<<(std::ostream& os, const TexturePosition& position)
{
    return os << "TexturePosition {x: " << position.x << ", y: " << position.y << '}';
}

}
