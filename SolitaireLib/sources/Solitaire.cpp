#include <string>

#include "cards/Card.h"
#include "Solitaire.h"
#include "archivers/HistoryTracker.h"
#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "cards/DeckGenerator.h"
#include "piles/FoundationPile.h"
#include "piles/StockPile.h"
#include "piles/TableauPile.h"

using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

Solitaire::Solitaire(std::unique_ptr<DeckGenerator> deckGenerator,
                     std::shared_ptr<StockPile> stockPile,
                     FoundationPiles foundationPiles,
                     TableauPiles tableauPiles,
                     std::unique_ptr<HistoryTracker> historyTracker,
                     std::unique_ptr<MoveCardsOperationSnapshotCreator>
                        moveCardsOperationSnapshotCreator):
    deckGenerator {std::move(deckGenerator)},
    stockPile {std::move(stockPile)},
    foundationPiles {std::move(foundationPiles)},
    tableauPiles {std::move(tableauPiles)},
    historyTracker {std::move(historyTracker)},
    moveCardsOperationSnapshotCreator {std::move(moveCardsOperationSnapshotCreator)} {
}

void Solitaire::startNewGame() {
    cardsInHand.clear();
    const auto deck = deckGenerator->generate();

    initializeFoundationPiles();
    const auto firstNotUsedCard = initializeTableauPilesAndReturnFirstNotUsedCard(deck);
    stockPile->initialize(firstNotUsedCard, deck.end());
}

void Solitaire::initializeFoundationPiles() {
    for (auto& pile: foundationPiles)
        pile->initialize();
}

Deck::const_iterator
Solitaire::initializeTableauPilesAndReturnFirstNotUsedCard(const Deck& deck) {
    auto firstNotUsedCard = deck.begin();
    unsigned currentPileCardsCount = 1;

    for (auto& pile: tableauPiles) {
        const auto firstPileCard = firstNotUsedCard;
        std::advance(firstNotUsedCard, currentPileCardsCount++);
        pile->initialize(firstPileCard, firstNotUsedCard);
    }

    return firstNotUsedCard;
}

void Solitaire::tryPullOutCardFromFoundationPile(const PileId id) {
    throwExceptionOnInvalidFoundationPileId(id);

    if (cardsInHand.empty()) {
        const auto pulledOutCard = foundationPiles[id]->tryPullOutCard();
        if (pulledOutCard)
            cardsInHand.push_back(*pulledOutCard);
    }
}

void Solitaire::tryAddCardOnFoundationPile(const piles::PileId id) {
    throwExceptionOnInvalidFoundationPileId(id);
}

void Solitaire::tryUncoverTableauPileTopCard(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);

    if (cardsInHand.empty())
        tableauPiles[id]->tryUncoverTopCard();
}

void Solitaire::tryPullOutCardsFromTableauPile(const PileId id, const unsigned quantity) {
    throwExceptionOnInvalidTableauPileId(id);

    if (cardsInHand.empty())
        cardsInHand = tableauPiles[id]->tryPullOutCards(quantity);
}

void Solitaire::tryAddCardsOnTableauPile(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);
}

void Solitaire::selectNextStockPileCard() {
    if (cardsInHand.empty())
        stockPile->selectNextCard();
}

void Solitaire::tryPullOutCardFromStockPile() {
    if (cardsInHand.empty()) {
        const auto pulledOutCard = stockPile->tryPullOutCard();
        if (pulledOutCard)
            cardsInHand.push_back(*pulledOutCard);
    }
}

const FoundationPile& Solitaire::getFoundationPile(const PileId id) const {
    throwExceptionOnInvalidFoundationPileId(id);
    return *foundationPiles[id];
}

const TableauPile& Solitaire::getTableauPile(const PileId id) const {
    throwExceptionOnInvalidTableauPileId(id);
    return *tableauPiles[id];
}

const StockPile& Solitaire::getStockPile() const {
    return *stockPile;
}

void Solitaire::throwExceptionOnInvalidFoundationPileId(const piles::PileId id) const {
    if (id.t >= foundationPilesCount)
        throw std::runtime_error{
            "Cannot access foundation pile with id: " + std::to_string(id)
        };
}

void Solitaire::throwExceptionOnInvalidTableauPileId(const piles::PileId id) const {
    if (id.t >= tableauPilesCount)
        throw std::runtime_error{
            "Cannot access tableau pile with id: " + std::to_string(id)
        };
}

const cards::Cards& Solitaire::getCardsInHand() const {
    return cardsInHand;
}

}
