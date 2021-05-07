#pragma once

#include <optional>

#include "cards/Card.h"
#include "cards/Deck.h"
#include "interfaces/Solitaire.h"

namespace solitaire::archivers::interfaces {
    class HistoryTracker;
    class MoveCardsOperationSnapshotCreator;
    class Snapshot;
}

namespace solitaire::cards::interfaces {
    class DeckGenerator;
}

namespace solitaire {

class Solitaire: public interfaces::Solitaire {
public:
    Solitaire(std::unique_ptr<cards::interfaces::DeckGenerator>,
              std::shared_ptr<piles::interfaces::StockPile>,
              FoundationPiles, TableauPiles,
              std::unique_ptr<archivers::interfaces::HistoryTracker>,
              std::unique_ptr<archivers::interfaces::MoveCardsOperationSnapshotCreator>);

    void startNewGame() override;

    void tryUndoOperation() override;
    void tryPutCardsBackFromHand() override;

    void tryPullOutCardFromFoundationPile(const piles::PileId) override;
    void tryAddCardOnFoundationPile(const piles::PileId) override;

    void tryUncoverTableauPileTopCard(const piles::PileId) override;
    void tryPullOutCardsFromTableauPile(
        const piles::PileId, const unsigned quantity) override;
    void tryAddCardsOnTableauPile(const piles::PileId) override;

    void trySelectNextStockPileCard() override;
    void tryPullOutCardFromStockPile() override;

    bool isGameFinished() const override;

    const piles::interfaces::FoundationPile&
    getFoundationPile(const piles::PileId) const override;
    const piles::interfaces::TableauPile&
    getTableauPile(const piles::PileId) const override;
    const piles::interfaces::StockPile& getStockPile() const override;
    const cards::Cards& getCardsInHand() const override;

private:
    using SnapshotPtr = std::unique_ptr<archivers::interfaces::Snapshot>;

    void initializeFoundationPiles();

    cards::Deck::const_iterator
    initializeTableauPilesAndReturnFirstNotUsedCard(const cards::Deck&);

    void tryAddPulledOutCardToHand(const std::optional<cards::Card>&, SnapshotPtr);
    void tryAddPulledOutCardsToHand(cards::Cards&&, SnapshotPtr);
    void tryAddCardOnFoundationPileFromHand(
        std::shared_ptr<piles::interfaces::FoundationPile>&);
    void tryAddCardOnTableauPileFromHand(
        std::shared_ptr<piles::interfaces::TableauPile>&);
    void saveHistoryIfCardMovedToOtherPileAndClearHand(
        const std::optional<cards::Card>&, SnapshotPtr);
    void saveHistoryIfCardMovedToOtherPile(SnapshotPtr);

    bool shouldUndoOperation() const;
    bool shouldAddCardOnFoundationPile() const;
    bool isCardAdded(const std::optional<cards::Card>&) const;
    bool shouldUncoverTableauPileTopCard(
        const std::shared_ptr<piles::interfaces::TableauPile>&) const;
    bool shouldSelectNextStockPileCard() const;
    bool isGameInProgressAndHandContainsCards() const;
    bool isGameInProgressAndHandIsEmpty() const;

    void throwExceptionOnInvalidFoundationPileId(const piles::PileId) const;
    void throwExceptionOnInvalidTableauPileId(const piles::PileId) const;

    std::unique_ptr<cards::interfaces::DeckGenerator> deckGenerator;
    std::shared_ptr<piles::interfaces::StockPile> stockPile;
    FoundationPiles foundationPiles;
    TableauPiles tableauPiles;
    std::unique_ptr<archivers::interfaces::HistoryTracker> historyTracker;
    std::unique_ptr<archivers::interfaces::MoveCardsOperationSnapshotCreator>
        moveCardsOperationSnapshotCreator;
    cards::Cards cardsInHand;
};

}
