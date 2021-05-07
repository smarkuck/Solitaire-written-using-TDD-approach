#include "cards/Card.h"
#include "cards/Value.h"
#include "piles/TableauPile.h"

using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

void TableauPile::initialize(const Deck::const_iterator& begin,
                             const Deck::const_iterator& end) {
    cards.assign(begin, end);
    topCoveredCardPosition = cards.empty() ? 0 : cards.size() - 1;
}

std::unique_ptr<archivers::interfaces::Snapshot> TableauPile::createSnapshot() {
    return std::make_unique<Snapshot>(shared_from_this(), cards,
                                      topCoveredCardPosition);
}

void TableauPile::tryUncoverTopCard() {
    if (isTopCardCovered())
        --topCoveredCardPosition;
}

void TableauPile::tryAddCards(Cards& cardsToAdd) {
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
        const Cards pulledOutCards {firstCardToPullOut, cards.end()};
        cards.erase(firstCardToPullOut, cards.end());
        return pulledOutCards;
    }

    return Cards {};
}

bool TableauPile::shouldPullOutCards(unsigned quantity) const {
    return cards.size() - topCoveredCardPosition >= quantity;
}

const Cards& TableauPile::getCards() const {
    return cards;
}

unsigned TableauPile::getTopCoveredCardPosition() const {
    return topCoveredCardPosition;
}

bool TableauPile::isTopCardCovered() const {
    return not cards.empty() and cards.size() == topCoveredCardPosition;
}

TableauPile::Snapshot::Snapshot(
    std::shared_ptr<TableauPile> tableauPile,
    Cards pileCards, unsigned topCoveredCardPosition):
    tableauPile {std::move(tableauPile)},
    pileCards {std::move(pileCards)},
    topCoveredCardPosition {std::move(topCoveredCardPosition)} {
}

void TableauPile::Snapshot::restore() const {
    tableauPile->cards = pileCards;
    tableauPile->topCoveredCardPosition = topCoveredCardPosition;
}

bool TableauPile::Snapshot::isSnapshotOfSameObject(
    const archivers::interfaces::Snapshot& snapshot) const
try {
    const auto& castedSnaphost =
        dynamic_cast<const Snapshot&>(snapshot);
    return tableauPile == castedSnaphost.tableauPile;
}
catch (const std::bad_cast&) {
    return false;
}

}
