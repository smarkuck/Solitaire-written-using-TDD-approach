#include <string>

#include "cards/Card.h"
#include "Solitaire.h"
#include "archivers/HistoryTracker.h"
#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "archivers/Snapshot.h"
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
        auto& pile = foundationPiles[id];
        auto snapshot = pile->createSnapshot();
        tryAddPulledOutCardToHand(pile->tryPullOutCard(), std::move(snapshot));
    }
}

void Solitaire::tryAddCardOnFoundationPile(const piles::PileId id) {
    throwExceptionOnInvalidFoundationPileId(id);
}

void Solitaire::tryUncoverTableauPileTopCard(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);

    auto& pile = tableauPiles[id];
    if (shouldTryUncoverTableauPileTopCard(pile)) {
        auto snapshot = pile->createSnapshot();
        pile->tryUncoverTopCard();
        historyTracker->save(std::move(snapshot));
    }
}

bool Solitaire::shouldTryUncoverTableauPileTopCard(
    const std::shared_ptr<TableauPile>& pile) const
{
    return cardsInHand.empty() and pile->isTopCardCovered();
}

void Solitaire::tryPullOutCardsFromTableauPile(const PileId id, const unsigned quantity) {
    throwExceptionOnInvalidTableauPileId(id);

    if (cardsInHand.empty()) {
        auto& pile = tableauPiles[id];
        auto snapshot = pile->createSnapshot();
        tryAddPulledOutCardsToHand(pile->tryPullOutCards(quantity), std::move(snapshot));
    }
}

void Solitaire::tryAddPulledOutCardsToHand(cards::Cards&& cards,
                                           std::unique_ptr<archivers::Snapshot> snapshot)
{
    if (not cards.empty()) {
        moveCardsOperationSnapshotCreator->saveSourcePileSnapshot(std::move(snapshot));
        cardsInHand = std::move(cards);
    }
}

void Solitaire::tryAddCardsOnTableauPile(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);
}

void Solitaire::trySelectNextStockPileCard() {
    if (shouldSelectNextStockPileCard()) {
        auto snapshot = stockPile->createSnapshot();
        stockPile->trySelectNextCard();
        historyTracker->save(std::move(snapshot));
    }
}

bool Solitaire::shouldSelectNextStockPileCard() const {
    return cardsInHand.empty() and not stockPile->getCards().empty();
}

void Solitaire::tryPullOutCardFromStockPile() {
    if (cardsInHand.empty()) {
        auto snapshot = stockPile->createSnapshot();
        tryAddPulledOutCardToHand(stockPile->tryPullOutCard(), std::move(snapshot));
    }
}

void Solitaire::tryAddPulledOutCardToHand(const std::optional<Card>& card,
                                          std::unique_ptr<Snapshot> snapshot)
{
    if (card) {
        moveCardsOperationSnapshotCreator->saveSourcePileSnapshot(std::move(snapshot));
        cardsInHand.push_back(card.value());
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
