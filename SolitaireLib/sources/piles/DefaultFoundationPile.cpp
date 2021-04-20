#include "cards/Card.h"
#include "cards/Value.h"
#include "piles/DefaultFoundationPile.h"

using namespace solitaire::cards;

namespace solitaire::piles {

void DefaultFoundationPile::initialize() {
    cards.clear();
}

std::unique_ptr<archivers::Snapshot> DefaultFoundationPile::createSnapshot() {
    return std::make_unique<Snapshot>(shared_from_this(), cards);
}

void DefaultFoundationPile::tryAddCard(std::optional<Card>& cardToAdd) {
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
        const auto pulledOutCard = cards.back();
        cards.pop_back();
        return pulledOutCard;
    }

    return std::nullopt;
}

const Cards& DefaultFoundationPile::getCards() const {
    return cards;
}

std::optional<Value> DefaultFoundationPile::getTopCardValue() const {
    if (cards.empty())
        return std::nullopt;
    return cards.back().getValue();
}

DefaultFoundationPile::Snapshot::Snapshot(
    std::shared_ptr<DefaultFoundationPile> foundationPile, Cards pileCards):
    foundationPile {std::move(foundationPile)},
    pileCards {std::move(pileCards)} {
}

void DefaultFoundationPile::Snapshot::restore() const {
    foundationPile->cards = pileCards;
}

bool DefaultFoundationPile::Snapshot::isSnapshotOfSameObject(
    const archivers::Snapshot& snapshot) const
try {
    const auto& castedSnaphost =
        dynamic_cast<const DefaultFoundationPile::Snapshot&>(snapshot);
    return foundationPile == castedSnaphost.foundationPile;
}
catch (const std::bad_cast&) {
    return false;
}

}
