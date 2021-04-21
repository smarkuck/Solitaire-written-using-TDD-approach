#pragma once

#include <array>
#include <memory>
#include <optional>

#include "cards/Cards.h"
#include "cards/Deck.h"
#include "piles/PileId.h"

namespace solitaire {

namespace archivers {
    class HistoryTracker;
    class MoveCardsOperationSnapshotCreator;
    class Snapshot;
}

namespace cards {
    class DeckGenerator;
}

namespace piles {
    class FoundationPile;
    class StockPile;
    class TableauPile;
}

class Solitaire {
private:
    static constexpr unsigned foundationPilesCount {4};
    static constexpr unsigned tableauPilesCount {7};

    using FoundationPiles =
        std::array<std::shared_ptr<piles::FoundationPile>, foundationPilesCount>;
    using TableauPiles =
        std::array<std::shared_ptr<piles::TableauPile>, tableauPilesCount>;

public:
    Solitaire(std::unique_ptr<cards::DeckGenerator>,
              std::shared_ptr<piles::StockPile>,
              FoundationPiles, TableauPiles,
              std::unique_ptr<archivers::HistoryTracker>,
              std::unique_ptr<archivers::MoveCardsOperationSnapshotCreator>);

    void startNewGame();
    void tryPutCardsBackFromHand();

    void tryPullOutCardFromFoundationPile(const piles::PileId);
    void tryAddCardOnFoundationPile(const piles::PileId);

    void tryUncoverTableauPileTopCard(const piles::PileId);
    void tryPullOutCardsFromTableauPile(const piles::PileId, const unsigned quantity);
    void tryAddCardsOnTableauPile(const piles::PileId);

    void trySelectNextStockPileCard();
    void tryPullOutCardFromStockPile();

    const piles::FoundationPile& getFoundationPile(const piles::PileId) const;
    const piles::TableauPile& getTableauPile(const piles::PileId) const;
    const piles::StockPile& getStockPile() const;
    const cards::Cards& getCardsInHand() const;

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

    bool shouldTryUncoverTableauPileTopCard(
        const std::shared_ptr<piles::TableauPile>&) const;
    bool shouldSelectNextStockPileCard() const;
    bool isOneCardInHand() const;
    bool isCardAdded(const std::optional<cards::Card>&) const;

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
