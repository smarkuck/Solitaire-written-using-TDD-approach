#include "cards/Card.h"
#include "piles/DefaultStockPile.h"

using namespace solitaire::cards;

namespace solitaire::piles {

void DefaultStockPile::initialize(const Cards::const_iterator& begin,
                                  const Cards::const_iterator& end)
{
    cards.assign(begin, end);
    selectedCardIndex.reset();
    lastPulledOutCard.reset();
}

void DefaultStockPile::selectNextCard() {
    lastPulledOutCard.reset();

    incrementSelectedCardIndex();
    if (selectedCardIndex == cards.size())
        selectedCardIndex.reset();
}

std::optional<Card> DefaultStockPile::tryPullOutCard() {
    if (selectedCardIndex) {
        lastPulledOutCard = cards.at(selectedCardIndex.value());
        cards.erase(std::next(cards.begin(), selectedCardIndex.value()));
        decrementSelectedCardIndex();
        return lastPulledOutCard;
    }

    lastPulledOutCard.reset();
    return std::nullopt;
}

void DefaultStockPile::incrementSelectedCardIndex() {
    selectedCardIndex = selectedCardIndex ? selectedCardIndex.value() + 1 : 0;
}

void DefaultStockPile::decrementSelectedCardIndex() {
    if (selectedCardIndex > 0u)
        --selectedCardIndex.value();
    else
        selectedCardIndex.reset();
}

void DefaultStockPile::tryRestoreLastPulledOutCard() {
    if (lastPulledOutCard) {
        restoreLastPulledOutCard();
        lastPulledOutCard.reset();
    }
}

void DefaultStockPile::restoreLastPulledOutCard() {
    incrementSelectedCardIndex();

    const auto placeToInsertCard =
        std::next(cards.begin(), selectedCardIndex.value());
    cards.insert(placeToInsertCard, lastPulledOutCard.value());
}

const Cards& DefaultStockPile::getCards() const {
    return cards;
}

std::optional<unsigned> DefaultStockPile::getSelectedCardIndex() const {
    return selectedCardIndex;
}

}
