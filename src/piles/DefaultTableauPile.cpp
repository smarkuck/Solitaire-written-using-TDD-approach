#include "piles/DefaultTableauPile.h"

using namespace solitaire::cards;

namespace solitaire {
namespace piles {

void DefaultTableauPile::initialize(const Cards::const_iterator& begin,
                                    const Cards::const_iterator& end) {
    cards.assign(begin, end);
    lastPulledOutCards.clear();
    placeInOrderOfFirstCoveredCard = cards.empty() ? 0 : cards.size() - 1;
}

void DefaultTableauPile::tryUncoverTopCard() {
    lastPulledOutCards.clear();

    if (isTopCardCovered())
        --placeInOrderOfFirstCoveredCard;
}

void DefaultTableauPile::tryAddCards(Cards& cardsToAdd) {
    lastPulledOutCards.clear();

    if (shouldAddCards(cardsToAdd)) {
        cards.insert(cards.end(), cardsToAdd.begin(), cardsToAdd.end());
        cardsToAdd.clear();
    }
}

bool DefaultTableauPile::shouldAddCards(const Cards& cardsToAdd) const {
    if (cardsToAdd.empty() or isTopCardCovered()) return false;
    if (cards.empty()) return isFirstCardToAddKing(cardsToAdd);
    return isFirstCardToAddCorrect(cardsToAdd);
}

bool DefaultTableauPile::isTopCardCovered() const {
    return not cards.empty() and cards.size() == placeInOrderOfFirstCoveredCard;
}

bool DefaultTableauPile::isFirstCardToAddKing(const Cards& cardsToAdd) const {
    return cardsToAdd.front().getValue() == Value::King;
}

bool DefaultTableauPile::isFirstCardToAddCorrect(const Cards& cardsToAdd) const {
    const Card& topPileCard = cards.back();
    const Card& firstCardToAdd = cardsToAdd.front();

    return topPileCard.hasValueOneGreaterThan(firstCardToAdd) and
           topPileCard.hasDifferentColorThan(firstCardToAdd);
}

Cards DefaultTableauPile::tryPullOutCards(unsigned quantity) {
    if (shouldPullOutCards(quantity)) {
        const auto firstCardToPullOut = std::prev(cards.end(), quantity);
        lastPulledOutCards.assign(firstCardToPullOut, cards.end());
        cards.erase(firstCardToPullOut, cards.end());
        return lastPulledOutCards;
    }

    lastPulledOutCards.clear();
    return Cards {};
}

bool DefaultTableauPile::shouldPullOutCards(unsigned quantity) const {
    return cards.size() - placeInOrderOfFirstCoveredCard >= quantity;
}

void DefaultTableauPile::tryRestoreLastPulledOutCards() {
    cards.insert(cards.end(), lastPulledOutCards.begin(), lastPulledOutCards.end());
    lastPulledOutCards.clear();
}

const Cards& DefaultTableauPile::getCards() const {
    return cards;
}

unsigned DefaultTableauPile::getPlaceInOrderOfFirstCoveredCard() const {
    return placeInOrderOfFirstCoveredCard;
}

}
}
