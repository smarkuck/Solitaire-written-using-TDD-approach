#pragma once

#include <ostream>

#include "Position.h"
#include "Size.h"

namespace solitaire::geometry {

struct Area {
    Position position;
    Size size;
};

inline bool operator==(const Area& lhs, const Area& rhs) {
    return lhs.position == rhs.position and lhs.size == rhs.size;
}

inline std::ostream& operator<<(std::ostream& os, const Area& area)
{
    return os << "Area {position: " << area.position
              << ", size: " << area.size << '}';
}

}
