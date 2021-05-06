#include "DefaultContext.h"
#include "Solitaire.h"
#include "colliders/FoundationPileCollider.h"
#include "piles/PileId.h"

using namespace solitaire::colliders;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire {

DefaultContext::DefaultContext(std::unique_ptr<Solitaire> solitaire,
                               FoundationPileColliders foundationPileColliders):
    solitaire {std::move(solitaire)},
    foundationPileColliders {std::move(foundationPileColliders)} {
}

void DefaultContext::setMousePosition(const Position& position) {
    mousePosition = position;
}

void DefaultContext::setCardsInHandPosition(const Position& position) {
    cardsInHandPosition = position;
}

Solitaire& DefaultContext::getSolitaire() {
    return *solitaire;
}

const Solitaire& DefaultContext::getSolitaire() const {
    return *solitaire;
}

FoundationPileCollider&
DefaultContext::getFoundationPileCollider(const PileId id) {
    if (id >= Solitaire::foundationPilesCount)
        throw std::runtime_error {"Invalid foundation pile id: " + id};
    return *foundationPileColliders[id];
}

const FoundationPileCollider&
DefaultContext::getFoundationPileCollider(const PileId id) const {
    if (id >= Solitaire::foundationPilesCount)
        throw std::runtime_error {"Invalid foundation pile id: " + id};
    return *foundationPileColliders[id];
}

Position DefaultContext::getMousePosition() const {
    return mousePosition;
}

Position DefaultContext::getCardsInHandPosition() const {
    return cardsInHandPosition;
}

}
