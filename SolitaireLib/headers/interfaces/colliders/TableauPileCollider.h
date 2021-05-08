#pragma once

#include <optional>

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::colliders::interfaces {

class TableauPileCollider {
public:
    virtual ~TableauPileCollider() = default;

    virtual std::optional<unsigned> tryGetCollidedCardIndex(
        const geometry::Position&) const = 0;

    virtual geometry::Position getCardPosition(const unsigned index) const = 0;
};

}
