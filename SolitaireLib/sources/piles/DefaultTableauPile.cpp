#include "cards/Card.h"
#include "cards/Value.h"
#include "piles/DefaultTableauPile.h"

using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

void DefaultTableauPile::initialize(const Deck::const_iterator& begin,
                                    const Deck::const_iterator& end) {
    cards.assign(begin, end);
    topCoveredCardPosition = cards.empty() ? 0 : cards.size() - 1;
}

std::unique_ptr<interfaces::Snapshot> DefaultTableauPile::createSnapshot() {
    return std::make_unique<Snapshot>(shared_from_this(), cards,
                                      topCoveredCardPosition);
}

void DefaultTableauPile::tryUncoverTopCard() {
    if (isTopCardCovered())
        --topCoveredCardPosition;
}

void DefaultTableauPile::tryAddCards(Cards& cardsToAdd) {
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
        const Cards pulledOutCards {firstCardToPullOut, cards.end()};
        cards.erase(firstCardToPullOut, cards.end());
        return pulledOutCards;
    }

    return Cards {};
}

bool DefaultTableauPile::shouldPullOutCards(unsigned quantity) const {
    return cards.size() - topCoveredCardPosition >= quantity;
}

const Cards& DefaultTableauPile::getCards() const {
    return cards;
}

unsigned DefaultTableauPile::getTopCoveredCardPosition() const {
    return topCoveredCardPosition;
}

bool DefaultTableauPile::isTopCardCovered() const {
    return not cards.empty() and cards.size() == topCoveredCardPosition;
}

DefaultTableauPile::Snapshot::Snapshot(
    std::shared_ptr<DefaultTableauPile> tableauPile,
    Cards pileCards, unsigned topCoveredCardPosition):
    tableauPile {std::move(tableauPile)},
    pileCards {std::move(pileCards)},
    topCoveredCardPosition {std::move(topCoveredCardPosition)} {
}

void DefaultTableauPile::Snapshot::restore() const {
    tableauPile->cards = pileCards;
    tableauPile->topCoveredCardPosition = topCoveredCardPosition;
}

bool DefaultTableauPile::Snapshot::isSnapshotOfSameObject(
    const interfaces::Snapshot& snapshot) const
try {
    const auto& castedSnaphost =
        dynamic_cast<const Snapshot&>(snapshot);
    return tableauPile == castedSnaphost.tableauPile;
}
catch (const std::bad_cast&) {
    return false;
}

}
