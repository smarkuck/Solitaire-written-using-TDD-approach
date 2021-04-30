#pragma once

#include <ostream>

#include "TexturePosition.h"
#include "TextureSize.h"

namespace solitaire::graphics {

struct TextureArea {
    TexturePosition position;
    TextureSize size;
};

inline bool operator==(const TextureArea& lhs, const TextureArea& rhs) {
    return lhs.position == rhs.position and lhs.size == rhs.size;
}

inline std::ostream& operator<<(std::ostream& os, const TextureArea& area)
{
    return os << "TextureArea {position: " << area.position
              << ", size: " << area.size << '}';
}

}
