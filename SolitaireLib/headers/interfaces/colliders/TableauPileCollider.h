#pragma once

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::colliders::interfaces {

class TableauPileCollider {
public:
    virtual ~TableauPileCollider() = default;
    virtual geometry::Position getCardPosition(const unsigned index) const = 0;
};

}
