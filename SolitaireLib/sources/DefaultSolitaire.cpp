#include <algorithm>
#include <string>

#include "DefaultSolitaire.h"
#include "archivers/HistoryTracker.h"
#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "cards/DeckGenerator.h"
#include "cards/Value.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/FoundationPile.h"
#include "piles/PileId.h"
#include "piles/StockPile.h"
#include "piles/TableauPile.h"

using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

DefaultSolitaire::DefaultSolitaire(std::unique_ptr<DeckGenerator> deckGenerator,
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

void DefaultSolitaire::startNewGame() {
    cardsInHand.clear();
    const auto deck = deckGenerator->generate();

    initializeFoundationPiles();
    const auto firstNotUsedCard = initializeTableauPilesAndReturnFirstNotUsedCard(deck);
    stockPile->initialize(firstNotUsedCard, deck.end());
}

void DefaultSolitaire::initializeFoundationPiles() {
    for (auto& pile: foundationPiles)
        pile->initialize();
}

Deck::const_iterator
DefaultSolitaire::initializeTableauPilesAndReturnFirstNotUsedCard(const Deck& deck) {
    auto firstNotUsedCard = deck.begin();
    unsigned currentPileCardsCount = 1;

    for (auto& pile: tableauPiles) {
        const auto firstPileCard = firstNotUsedCard;
        std::advance(firstNotUsedCard, currentPileCardsCount++);
        pile->initialize(firstPileCard, firstNotUsedCard);
    }

    return firstNotUsedCard;
}

void DefaultSolitaire::tryUndoOperation() {
    if (shouldUndoOperation())
        historyTracker->undo();
}

bool DefaultSolitaire::shouldUndoOperation() const {
    return isGameInProgressAndHandIsEmpty() and historyTracker->getHistorySize() > 0;
}

void DefaultSolitaire::tryPutCardsBackFromHand() {
    if (isGameInProgressAndHandContainsCards()) {
        moveCardsOperationSnapshotCreator->restoreSourcePile();
        cardsInHand.clear();
    }
}

void DefaultSolitaire::tryPullOutCardFromFoundationPile(const PileId id) {
    throwExceptionOnInvalidFoundationPileId(id);

    if (isGameInProgressAndHandIsEmpty()) {
        auto& pile = foundationPiles[id];
        auto snapshot = pile->createSnapshot();
        tryAddPulledOutCardToHand(pile->tryPullOutCard(), std::move(snapshot));
    }
}

void DefaultSolitaire::tryAddCardOnFoundationPile(const piles::PileId id) {
    throwExceptionOnInvalidFoundationPileId(id);

    if (shouldAddCardOnFoundationPile())
        tryAddCardOnFoundationPileFromHand(foundationPiles[id]);
}

bool DefaultSolitaire::shouldAddCardOnFoundationPile() const {
    return not isGameFinished() and cardsInHand.size() == 1;
}

void DefaultSolitaire::tryAddCardOnFoundationPileFromHand(
    std::shared_ptr<piles::FoundationPile>& pile)
{
    auto snapshot = pile->createSnapshot();
    std::optional<Card> cardToAdd {cardsInHand.back()};
    pile->tryAddCard(cardToAdd);
    saveHistoryIfCardMovedToOtherPileAndClearHand(cardToAdd, std::move(snapshot));
}

void DefaultSolitaire::saveHistoryIfCardMovedToOtherPileAndClearHand(
    const std::optional<Card>& cardToAdd,
    SnapshotPtr destinationPileSnapshot)
{
    if (isCardAdded(cardToAdd)) {
        saveHistoryIfCardMovedToOtherPile(std::move(destinationPileSnapshot));
        cardsInHand.clear();
    }
}

bool DefaultSolitaire::isCardAdded(const std::optional<Card>& card) const {
    return not card;
}

void DefaultSolitaire::tryUncoverTableauPileTopCard(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);

    auto& pile = tableauPiles[id];
    if (shouldUncoverTableauPileTopCard(pile)) {
        auto snapshot = pile->createSnapshot();
        pile->tryUncoverTopCard();
        historyTracker->save(std::move(snapshot));
    }
}

bool DefaultSolitaire::shouldUncoverTableauPileTopCard(
    const std::shared_ptr<TableauPile>& pile) const
{
    return isGameInProgressAndHandIsEmpty() and pile->isTopCardCovered();
}

void DefaultSolitaire::tryPullOutCardsFromTableauPile(
    const PileId id, const unsigned quantity)
{
    throwExceptionOnInvalidTableauPileId(id);

    if (isGameInProgressAndHandIsEmpty()) {
        auto& pile = tableauPiles[id];
        auto snapshot = pile->createSnapshot();
        tryAddPulledOutCardsToHand(pile->tryPullOutCards(quantity), std::move(snapshot));
    }
}

void DefaultSolitaire::tryAddPulledOutCardsToHand(
    cards::Cards&& cards, SnapshotPtr snapshot)
{
    if (not cards.empty()) {
        moveCardsOperationSnapshotCreator->saveSourcePileSnapshot(std::move(snapshot));
        cardsInHand = std::move(cards);
    }
}

void DefaultSolitaire::tryAddCardsOnTableauPile(const piles::PileId id) {
    throwExceptionOnInvalidTableauPileId(id);

    if (isGameInProgressAndHandContainsCards())
        tryAddCardOnTableauPileFromHand(tableauPiles[id]);
}

bool DefaultSolitaire::isGameInProgressAndHandContainsCards() const {
    return not isGameFinished() and not cardsInHand.empty();
}

void DefaultSolitaire::tryAddCardOnTableauPileFromHand(
    std::shared_ptr<piles::TableauPile>& pile)
{
    auto snapshot = pile->createSnapshot();
    pile->tryAddCards(cardsInHand);

    if (cardsInHand.empty())
        saveHistoryIfCardMovedToOtherPile(std::move(snapshot));
}

void DefaultSolitaire::saveHistoryIfCardMovedToOtherPile(
    SnapshotPtr destinationPileSnapshot)
{
    auto moveCardsOperationSnapshot =
        moveCardsOperationSnapshotCreator->createSnapshotIfCardsMovedToOtherPile(
            std::move(destinationPileSnapshot));

    if (moveCardsOperationSnapshot)
        historyTracker->save(std::move(moveCardsOperationSnapshot));
}

void DefaultSolitaire::trySelectNextStockPileCard() {
    if (shouldSelectNextStockPileCard()) {
        auto snapshot = stockPile->createSnapshot();
        stockPile->trySelectNextCard();
        historyTracker->save(std::move(snapshot));
    }
}

bool DefaultSolitaire::shouldSelectNextStockPileCard() const {
    return isGameInProgressAndHandIsEmpty() and not stockPile->getCards().empty();
}

void DefaultSolitaire::tryPullOutCardFromStockPile() {
    if (isGameInProgressAndHandIsEmpty()) {
        auto snapshot = stockPile->createSnapshot();
        tryAddPulledOutCardToHand(stockPile->tryPullOutCard(), std::move(snapshot));
    }
}

bool DefaultSolitaire::isGameInProgressAndHandIsEmpty() const {
    return not isGameFinished() and cardsInHand.empty();
}

void DefaultSolitaire::tryAddPulledOutCardToHand(
    const std::optional<Card>& card, SnapshotPtr snapshot)
{
    if (card) {
        moveCardsOperationSnapshotCreator->saveSourcePileSnapshot(std::move(snapshot));
        cardsInHand.push_back(card.value());
    }
}

bool DefaultSolitaire::isGameFinished() const {
    return std::all_of(foundationPiles.begin(), foundationPiles.end(),
        [](const auto& pile) {
            return pile->getTopCardValue() == Value::King;
        }
    );
}

const FoundationPile& DefaultSolitaire::getFoundationPile(const PileId id) const {
    throwExceptionOnInvalidFoundationPileId(id);
    return *foundationPiles[id];
}

const TableauPile& DefaultSolitaire::getTableauPile(const PileId id) const {
    throwExceptionOnInvalidTableauPileId(id);
    return *tableauPiles[id];
}

const StockPile& DefaultSolitaire::getStockPile() const {
    return *stockPile;
}

void DefaultSolitaire::throwExceptionOnInvalidFoundationPileId(
    const piles::PileId id) const
{
    if (id.t >= foundationPilesCount)
        throw std::runtime_error {
            "Cannot access foundation pile with id: " + std::to_string(id)
        };
}

void DefaultSolitaire::throwExceptionOnInvalidTableauPileId(
    const piles::PileId id) const
{
    if (id.t >= tableauPilesCount)
        throw std::runtime_error {
            "Cannot access tableau pile with id: " + std::to_string(id)
        };
}

const cards::Cards& DefaultSolitaire::getCardsInHand() const {
    return cardsInHand;
}

}
