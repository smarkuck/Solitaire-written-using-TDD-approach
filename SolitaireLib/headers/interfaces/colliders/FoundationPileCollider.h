#pragma once

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::colliders::interfaces {

class FoundationPileCollider {
public:
    virtual ~FoundationPileCollider() = default;
    virtual bool collidesWithPoint(const geometry::Position&) const = 0;
    virtual bool collidesWithCardsInHand(const geometry::Position&) const = 0;
    virtual geometry::Position getPosition() const = 0;
};

}
