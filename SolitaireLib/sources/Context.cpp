#include "Context.h"
#include "interfaces/Solitaire.h"
#include "interfaces/colliders/FoundationPileCollider.h"
#include "piles/PileId.h"

using namespace solitaire::colliders::interfaces;
using namespace solitaire::interfaces;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire {

Context::Context(std::unique_ptr<Solitaire> solitaire,
                 FoundationPileColliders foundationPileColliders):
    solitaire {std::move(solitaire)},
    foundationPileColliders {std::move(foundationPileColliders)} {
}

void Context::setMousePosition(const Position& position) {
    mousePosition = position;
}

void Context::setCardsInHandPosition(const Position& position) {
    cardsInHandPosition = position;
}

Solitaire& Context::getSolitaire() {
    return *solitaire;
}

const Solitaire& Context::getSolitaire() const {
    return *solitaire;
}

FoundationPileCollider& Context::getFoundationPileCollider(const PileId id) {
    if (id >= Solitaire::foundationPilesCount)
        throw std::runtime_error {"Invalid foundation pile id: " + id};
    return *foundationPileColliders[id];
}

const FoundationPileCollider& Context::getFoundationPileCollider(const PileId id) const {
    if (id >= Solitaire::foundationPilesCount)
        throw std::runtime_error {"Invalid foundation pile id: " + id};
    return *foundationPileColliders[id];
}

Position Context::getMousePosition() const {
    return mousePosition;
}

Position Context::getCardsInHandPosition() const {
    return cardsInHandPosition;
}

}
