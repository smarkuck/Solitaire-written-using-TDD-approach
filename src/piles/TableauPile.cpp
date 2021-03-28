#include "piles/TableauPile.h"

using namespace solitaire::card;

namespace solitaire {
namespace piles {

TableauPile::TableauPile(const Cards::const_iterator& begin,
                         const Cards::const_iterator& end)
{
    reset(begin, end);
}

void TableauPile::reset(card::Cards::const_iterator begin,
                        const card::Cards::const_iterator& end) {
    cards.assign(begin, end);
    lastPulledOutCards.clear();
    placeInOrderOfFirstCoveredCard = cards.size() > 0 ? cards.size() - 1 : 0;
}

void TableauPile::tryUncoverTopCard() {
    lastPulledOutCards.clear();

    if (isTopCardCovered())
        --placeInOrderOfFirstCoveredCard;
}

void TableauPile::tryAddCards(Cards& cardsToAdd) {
    lastPulledOutCards.clear();

    if (shouldAddCards(cardsToAdd)) {
        cards.insert(cards.end(), cardsToAdd.begin(), cardsToAdd.end());
        cardsToAdd.clear();
    }
}

bool TableauPile::shouldAddCards(const Cards& cardsToAdd) const {
    if (cardsToAdd.empty() or isTopCardCovered()) return false;
    if (cards.empty()) return isFirstCardToAddKing(cardsToAdd);
    return isFirstCardToAddCorrect(cardsToAdd);
}

bool TableauPile::isTopCardCovered() const {
    return cards.size() > 0 and cards.size() == placeInOrderOfFirstCoveredCard;
}

bool TableauPile::isFirstCardToAddKing(const Cards& cardsToAdd) const {
    return cardsToAdd.front().getValue() == Value::King;
}

bool TableauPile::isFirstCardToAddCorrect(const Cards& cardsToAdd) const {
    const Card& topPileCard = cards.back();
    const Card& firstCardToAdd = cardsToAdd.front();

    return topPileCard.hasValueOneGreaterThan(firstCardToAdd) and
           topPileCard.hasDifferentColorThan(firstCardToAdd);
}

Cards TableauPile::tryPullOutCards(unsigned quantity) {
    if (shouldPullOutCards(quantity)) {
        const auto firstCardToPullOut = std::prev(cards.end(), quantity);
        lastPulledOutCards.assign(firstCardToPullOut, cards.end());
        cards.erase(firstCardToPullOut, cards.end());
        return lastPulledOutCards;
    }

    lastPulledOutCards.clear();
    return Cards {};
}

bool TableauPile::shouldPullOutCards(unsigned quantity) const {
    return cards.size() - placeInOrderOfFirstCoveredCard >= quantity;
}

void TableauPile::tryRestoreLastPulledOutCards() {
    cards.insert(cards.end(), lastPulledOutCards.begin(), lastPulledOutCards.end());
    lastPulledOutCards.clear();
}

const Cards& TableauPile::getCards() const {
    return cards;
}

unsigned TableauPile::getPlaceInOrderOfFirstCoveredCard() const {
    return placeInOrderOfFirstCoveredCard;
}

}
}
