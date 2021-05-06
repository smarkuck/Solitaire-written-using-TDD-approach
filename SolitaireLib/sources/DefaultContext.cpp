#include "DefaultContext.h"
#include "Solitaire.h"

using namespace solitaire::geometry;

namespace solitaire {

DefaultContext::DefaultContext(std::unique_ptr<Solitaire> solitaire):
    solitaire {std::move(solitaire)} {
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

Position DefaultContext::getMousePosition() const {
    return mousePosition;
}

Position DefaultContext::getCardsInHandPosition() const {
    return cardsInHandPosition;
}

}
