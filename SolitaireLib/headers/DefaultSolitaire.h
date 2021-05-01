#pragma once

#include <optional>

#include "Solitaire.h"
#include "cards/Card.h"
#include "cards/Deck.h"

namespace solitaire {

namespace archivers {
    class HistoryTracker;
    class MoveCardsOperationSnapshotCreator;
    class Snapshot;
}

namespace cards {
    class DeckGenerator;
}

class DefaultSolitaire: public Solitaire {
public:
    DefaultSolitaire(std::unique_ptr<cards::DeckGenerator>,
                     std::shared_ptr<piles::StockPile>,
                     FoundationPiles, TableauPiles,
                     std::unique_ptr<archivers::HistoryTracker>,
                     std::unique_ptr<archivers::MoveCardsOperationSnapshotCreator>);

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

    const piles::FoundationPile& getFoundationPile(const piles::PileId) const override;
    const piles::TableauPile& getTableauPile(const piles::PileId) const override;
    const piles::StockPile& getStockPile() const override;
    const cards::Cards& getCardsInHand() const override;

private:
    using SnapshotPtr = std::unique_ptr<archivers::Snapshot>;

    void initializeFoundationPiles();

    cards::Deck::const_iterator
    initializeTableauPilesAndReturnFirstNotUsedCard(const cards::Deck&);

    void tryAddPulledOutCardToHand(const std::optional<cards::Card>&, SnapshotPtr);
    void tryAddPulledOutCardsToHand(cards::Cards&&, SnapshotPtr);
    void tryAddCardOnFoundationPileFromHand(std::shared_ptr<piles::FoundationPile>&);
    void tryAddCardOnTableauPileFromHand(std::shared_ptr<piles::TableauPile>&);
    void saveHistoryIfCardMovedToOtherPileAndClearHand(
        const std::optional<cards::Card>&, SnapshotPtr);
    void saveHistoryIfCardMovedToOtherPile(SnapshotPtr);

    bool shouldUndoOperation() const;
    bool shouldAddCardOnFoundationPile() const;
    bool isCardAdded(const std::optional<cards::Card>&) const;
    bool shouldUncoverTableauPileTopCard(
        const std::shared_ptr<piles::TableauPile>&) const;
    bool shouldSelectNextStockPileCard() const;
    bool isGameInProgressAndHandContainsCards() const;
    bool isGameInProgressAndHandIsEmpty() const;

    void throwExceptionOnInvalidFoundationPileId(const piles::PileId) const;
    void throwExceptionOnInvalidTableauPileId(const piles::PileId) const;

    std::unique_ptr<cards::DeckGenerator> deckGenerator;
    std::shared_ptr<piles::StockPile> stockPile;
    FoundationPiles foundationPiles;
    TableauPiles tableauPiles;
    std::unique_ptr<archivers::HistoryTracker> historyTracker;
    std::unique_ptr<archivers::MoveCardsOperationSnapshotCreator>
        moveCardsOperationSnapshotCreator;
    cards::Cards cardsInHand;
};

}
