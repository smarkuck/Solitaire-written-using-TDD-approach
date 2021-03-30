#include "piles/FoundationPile.h"

using namespace solitaire::cards;

namespace solitaire {
namespace piles {

void FoundationPile::initialize() {
    cards.clear();
    lastPulledOutCard.reset();
}

void FoundationPile::tryAddCard(std::optional<Card>& cardToAdd) {
    lastPulledOutCard.reset();

    if (shouldAddCard(cardToAdd)) {
        cards.push_back(cardToAdd.value());
        cardToAdd.reset();
    }
}

bool FoundationPile::shouldAddCard(std::optional<Card>& cardToAdd) const {
    if (not cardToAdd) return false;
    if (cards.empty()) return isCardToAddAce(cardToAdd.value());
    return isCardToAddCorrect(cardToAdd.value());
}

bool FoundationPile::isCardToAddAce(const Card& cardToAdd) const {
    return cardToAdd.getValue() == Value::Ace;
}

bool FoundationPile::isCardToAddCorrect(const Card& cardToAdd) const {
    const Card& topPileCard = cards.back();
    return cardToAdd.hasValueOneGreaterThan(topPileCard) and
           cardToAdd.hasSameSuitAs(topPileCard);
}

std::optional<Card> FoundationPile::tryPullOutCard() {
    if (not cards.empty()) {
        lastPulledOutCard = cards.back();
        cards.pop_back();
        return lastPulledOutCard;
    }

    lastPulledOutCard.reset();
    return std::nullopt;
}

void FoundationPile::tryRestoreLastPulledOutCard() {
    if (lastPulledOutCard) {
        cards.push_back(lastPulledOutCard.value());
        lastPulledOutCard.reset();
    }
}

const Cards& FoundationPile::getCards() const {
    return cards;
}

std::optional<Value> FoundationPile::getTopCardValue() const {
    if (cards.empty())
        return std::nullopt;
    return cards.back().getValue();
}

}
}
