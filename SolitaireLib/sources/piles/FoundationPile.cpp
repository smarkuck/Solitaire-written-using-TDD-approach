#include "cards/Card.h"
#include "cards/Value.h"
#include "piles/FoundationPile.h"

using namespace solitaire::cards;

namespace solitaire::piles {

void FoundationPile::initialize() {
    cards.clear();
}

std::unique_ptr<archivers::interfaces::Snapshot> FoundationPile::createSnapshot() {
    return std::make_unique<Snapshot>(shared_from_this(), cards);
}

void FoundationPile::tryAddCard(std::optional<Card>& cardToAdd) {
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
        const auto pulledOutCard = cards.back();
        cards.pop_back();
        return pulledOutCard;
    }

    return std::nullopt;
}

const Cards& FoundationPile::getCards() const {
    return cards;
}

std::optional<Value> FoundationPile::getTopCardValue() const {
    if (cards.empty())
        return std::nullopt;
    return cards.back().getValue();
}

FoundationPile::Snapshot::Snapshot(
    std::shared_ptr<FoundationPile> foundationPile, Cards pileCards):
    foundationPile {std::move(foundationPile)},
    pileCards {std::move(pileCards)} {
}

void FoundationPile::Snapshot::restore() const {
    foundationPile->cards = pileCards;
}

bool FoundationPile::Snapshot::isSnapshotOfSameObject(
    const archivers::interfaces::Snapshot& snapshot) const
try {
    const auto& castedSnaphost =
        dynamic_cast<const Snapshot&>(snapshot);
    return foundationPile == castedSnaphost.foundationPile;
}
catch (const std::bad_cast&) {
    return false;
}

}
