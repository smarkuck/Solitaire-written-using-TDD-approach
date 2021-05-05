#pragma once

#include <ostream>

namespace solitaire::geometry {

struct Size {
    int width, height;
};

inline bool operator==(const Size& lhs, const Size& rhs) {
    return lhs.width == rhs.width and lhs.height == rhs.height;
}

inline std::ostream& operator<<(std::ostream& os, const Size& size)
{
    return os << "Size {width: " << size.width
              << ", height: " << size.height << '}';
}

}
