#include "cards/Card.h"
#include "cards/Value.h"
#include "piles/DefaultFoundationPile.h"

using namespace solitaire::cards;

namespace solitaire {
namespace piles {

void DefaultFoundationPile::initialize() {
    cards.clear();
    lastPulledOutCard.reset();
}

void DefaultFoundationPile::tryAddCard(std::optional<Card>& cardToAdd) {
    lastPulledOutCard.reset();

    if (shouldAddCard(cardToAdd)) {
        cards.push_back(cardToAdd.value());
        cardToAdd.reset();
    }
}

bool DefaultFoundationPile::shouldAddCard(std::optional<Card>& cardToAdd) const {
    if (not cardToAdd) return false;
    if (cards.empty()) return isCardToAddAce(cardToAdd.value());
    return isCardToAddCorrect(cardToAdd.value());
}

bool DefaultFoundationPile::isCardToAddAce(const Card& cardToAdd) const {
    return cardToAdd.getValue() == Value::Ace;
}

bool DefaultFoundationPile::isCardToAddCorrect(const Card& cardToAdd) const {
    const Card& topPileCard = cards.back();
    return cardToAdd.hasValueOneGreaterThan(topPileCard) and
           cardToAdd.hasSameSuitAs(topPileCard);
}

std::optional<Card> DefaultFoundationPile::tryPullOutCard() {
    if (not cards.empty()) {
        lastPulledOutCard = cards.back();
        cards.pop_back();
        return lastPulledOutCard;
    }

    lastPulledOutCard.reset();
    return std::nullopt;
}

void DefaultFoundationPile::tryRestoreLastPulledOutCard() {
    if (lastPulledOutCard) {
        cards.push_back(lastPulledOutCard.value());
        lastPulledOutCard.reset();
    }
}

const Cards& DefaultFoundationPile::getCards() const {
    return cards;
}

std::optional<Value> DefaultFoundationPile::getTopCardValue() const {
    if (cards.empty())
        return std::nullopt;
    return cards.back().getValue();
}

}
}
