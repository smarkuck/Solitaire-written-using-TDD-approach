#include "piles/StockPile.h"

using namespace solitaire::card;

namespace solitaire {
namespace piles {

void StockPile::initialize(const Cards::const_iterator& begin,
                           const Cards::const_iterator& end)
{
    cards.assign(begin, end);
    selectedCardIndex.reset();
    lastPulledOutCard.reset();
}

void StockPile::selectNextCard() {
    lastPulledOutCard.reset();

    incrementSelectedCardIndex();
    if (selectedCardIndex == cards.size())
        selectedCardIndex.reset();
}

std::optional<Card> StockPile::tryPullOutCard() {
    if (selectedCardIndex) {
        lastPulledOutCard = cards.at(selectedCardIndex.value());
        cards.erase(std::next(cards.begin(), selectedCardIndex.value()));
        decrementSelectedCardIndex();
        return lastPulledOutCard;
    }

    lastPulledOutCard.reset();
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

void StockPile::tryRestoreLastPulledOutCard() {
    if (lastPulledOutCard) {
        restoreLastPulledOutCard();
        lastPulledOutCard.reset();
    }
}

void StockPile::restoreLastPulledOutCard() {
    incrementSelectedCardIndex();

    const auto placeToInsertCard =
        std::next(cards.begin(), selectedCardIndex.value());
    cards.insert(placeToInsertCard, lastPulledOutCard.value());
}

const Cards& StockPile::getCards() const {
    return cards;
}

std::optional<unsigned> StockPile::getSelectedCardIndex() const {
    return selectedCardIndex;
}

}
}
