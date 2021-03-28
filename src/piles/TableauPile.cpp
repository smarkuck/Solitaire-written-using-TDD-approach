#include "piles/TableauPile.h"

using namespace solitaire::card;

namespace solitaire {
namespace piles {

TableauPile::TableauPile(Cards::const_iterator begin, const Cards::const_iterator& end) {
    cards.assign(begin, end);
    queuePositionOfFirstFaceDownCard = cards.size() > 0 ? cards.size() - 1 : 0;
}

void TableauPile::tryAddCards(Cards& cardsToAdd) {
    if (shouldAddCards(cardsToAdd)) {
        cards.insert(cards.end(), cardsToAdd.begin(), cardsToAdd.end());
        cardsToAdd.clear();
    }
}

bool TableauPile::shouldAddCards(const Cards& cardsToAdd) const {
    if (cardsToAdd.empty()) return false;
    if (cards.empty()) return isFirstCardKing(cardsToAdd);
    return isFirstCardToAddCorrect(cardsToAdd);
}

bool TableauPile::isFirstCardKing(const Cards& cardsToAdd) const {
    return cardsToAdd.front().getValue() == Value::King;
}

bool TableauPile::isFirstCardToAddCorrect(const Cards& cardsToAdd) const {
    const Card& topPileCard = cards.back();
    const Card& firstCardToAdd = cardsToAdd.front();

    return topPileCard.hasValueOneGreaterThan(firstCardToAdd) and
            topPileCard.hasDifferentColorThan(firstCardToAdd);
}

const Cards& TableauPile::getCards() const {
    return cards;
}

unsigned TableauPile::getQueuePositionOfFirstFaceDownCard() const {
    return queuePositionOfFirstFaceDownCard;
}

}
}
