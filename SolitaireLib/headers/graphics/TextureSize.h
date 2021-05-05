#pragma once

#include <ostream>

namespace solitaire::graphics {

struct TextureSize {
    int width, height;
};

inline bool operator==(const TextureSize& lhs, const TextureSize& rhs) {
    return lhs.width == rhs.width and lhs.height == rhs.height;
}

inline std::ostream& operator<<(std::ostream& os, const TextureSize& size)
{
    return os << "TextureSize {width: " << size.width
              << ", height: " << size.height << '}';
}

}
