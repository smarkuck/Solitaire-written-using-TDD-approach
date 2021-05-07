#pragma once

namespace solitaire {

class Solitaire;

namespace colliders::interfaces {
class FoundationPileCollider;
}

namespace geometry {
struct Position;
}

namespace piles {
struct PileId;
}

class Context {
public:
    virtual ~Context() = default;

    virtual void setMousePosition(const geometry::Position&) = 0;
    virtual void setCardsInHandPosition(const geometry::Position&) = 0;

    virtual Solitaire& getSolitaire() = 0;
    virtual const Solitaire& getSolitaire() const = 0;

    virtual colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) = 0;

    virtual const colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) const = 0;

    virtual geometry::Position getMousePosition() const = 0;
    virtual geometry::Position getCardsInHandPosition() const = 0;
};

}
