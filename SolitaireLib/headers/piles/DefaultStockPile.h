#pragma once

#include "StockPile.h"
#include "interfaces/archivers/Snapshot.h"

namespace solitaire::piles {

class DefaultStockPile: public std::enable_shared_from_this<DefaultStockPile>,
                        public StockPile {
public:
    void initialize(const cards::Deck::const_iterator& begin,
                    const cards::Deck::const_iterator& end) override;

    std::unique_ptr<archivers::interfaces::Snapshot> createSnapshot() override;

    void trySelectNextCard() override;
    std::optional<cards::Card> tryPullOutCard() override;

    const cards::Cards& getCards() const override;
    std::optional<unsigned> getSelectedCardIndex() const override;

private:
    class Snapshot;

    void incrementSelectedCardIndex();
    void decrementSelectedCardIndex();

    cards::Cards cards;
    std::optional<unsigned> selectedCardIndex;
};

class DefaultStockPile::Snapshot: public archivers::interfaces::Snapshot {
public:
    Snapshot(std::shared_ptr<DefaultStockPile>, cards::Cards,
             std::optional<unsigned> selectedCardIndex);

    void restore() const override;
    bool isSnapshotOfSameObject(
        const archivers::interfaces::Snapshot&) const override;

private:
    const std::shared_ptr<DefaultStockPile> stockPile;
    const cards::Cards pileCards;
    const std::optional<unsigned> selectedCardIndex;
};

}
