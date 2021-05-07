#pragma once

namespace solitaire::colliders::interfaces {
class FoundationPileCollider;
}

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::piles {
struct PileId;
}

namespace solitaire::interfaces {
class Solitaire;

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
