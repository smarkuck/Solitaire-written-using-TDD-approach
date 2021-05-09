#pragma once

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::colliders::interfaces {

class StockPileCollider {
public:
    virtual ~StockPileCollider() = default;
    virtual bool coveredCardsCollidesWith(const geometry::Position&) const = 0;
    virtual bool uncoveredCardsCollidesWith(const geometry::Position&) const = 0;
    virtual geometry::Position getCoveredCardsPosition() const = 0;
    virtual geometry::Position getUncoveredCardsPosition() const = 0;
};

}
