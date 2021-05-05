#include "DefaultContext.h"

using namespace solitaire::graphics;

namespace solitaire {

void DefaultContext::setMousePosition(const TexturePosition& position) {
    mousePosition = position;
}

void DefaultContext::setCardsInHandPosition(const TexturePosition& position) {
    cardsInHandPosition = position;
}

TexturePosition DefaultContext::getMousePosition() const {
    return mousePosition;
}

TexturePosition DefaultContext::getCardsInHandPosition() const {
    return cardsInHandPosition;
}

}
