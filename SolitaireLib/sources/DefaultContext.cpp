#include "DefaultContext.h"

using namespace solitaire::geometry;

namespace solitaire {

void DefaultContext::setMousePosition(const Position& position) {
    mousePosition = position;
}

void DefaultContext::setCardsInHandPosition(const Position& position) {
    cardsInHandPosition = position;
}

Position DefaultContext::getMousePosition() const {
    return mousePosition;
}

Position DefaultContext::getCardsInHandPosition() const {
    return cardsInHandPosition;
}

}
