#include "cards/Card.h"
#include "piles/StockPile.h"

using namespace solitaire::archivers;
using namespace solitaire::cards;

namespace solitaire::piles {

void StockPile::initialize(const Deck::const_iterator& begin,
                           const Deck::const_iterator& end)
{
    cards.assign(begin, end);
    selectedCardIndex.reset();
}

std::unique_ptr<archivers::interfaces::Snapshot> StockPile::createSnapshot() {
    return std::make_unique<Snapshot>(shared_from_this(), cards, selectedCardIndex);
}

void StockPile::trySelectNextCard() {
    incrementSelectedCardIndex();
    if (selectedCardIndex == cards.size())
        selectedCardIndex.reset();
}

std::optional<Card> StockPile::tryPullOutCard() {
    if (selectedCardIndex) {
        const auto pulledOutCard = cards.at(selectedCardIndex.value());
        cards.erase(std::next(cards.begin(), selectedCardIndex.value()));
        decrementSelectedCardIndex();
        return pulledOutCard;
    }

    return std::nullopt;
}

void StockPile::incrementSelectedCardIndex() {
    selectedCardIndex = selectedCardIndex ? selectedCardIndex.value() + 1 : 0;
}

void StockPile::decrementSelectedCardIndex() {
    if (selectedCardIndex > 0u)
        --selectedCardIndex.value();
    else
        selectedCardIndex.reset();
}

const Cards& StockPile::getCards() const {
    return cards;
}

std::optional<unsigned> StockPile::getSelectedCardIndex() const {
    return selectedCardIndex;
}

StockPile::Snapshot::Snapshot(
    std::shared_ptr<StockPile> stockPile, Cards pileCards,
    std::optional<unsigned> selectedCardIndex):
    stockPile {std::move(stockPile)},
    pileCards {std::move(pileCards)},
    selectedCardIndex {std::move(selectedCardIndex)} {
}

void StockPile::Snapshot::restore() const {
    stockPile->cards = pileCards;
    stockPile->selectedCardIndex = selectedCardIndex;
}

bool StockPile::Snapshot::isSnapshotOfSameObject(
    const archivers::interfaces::Snapshot& snapshot) const
try {
    const auto& castedSnaphost =
        dynamic_cast<const Snapshot&>(snapshot);
    return stockPile == castedSnaphost.stockPile;
}
catch (const std::bad_cast&) {
    return false;
}

}
