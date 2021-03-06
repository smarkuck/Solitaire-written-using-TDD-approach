#pragma once

#include "interfaces/archivers/Snapshot.h"
#include "interfaces/piles/TableauPile.h"

namespace solitaire::piles {

class TableauPile: public std::enable_shared_from_this<TableauPile>,
                   public interfaces::TableauPile {
public:
    void initialize(const cards::Deck::const_iterator& begin,
                    const cards::Deck::const_iterator& end) override;

    std::unique_ptr<archivers::interfaces::Snapshot> createSnapshot() override;

    void tryUncoverTopCard() override;
    void tryAddCards(cards::Cards& cardsToAdd) override;
    cards::Cards tryPullOutCards(unsigned quantity) override;

    const cards::Cards& getCards() const override;
    unsigned getTopCoveredCardPosition() const override;
    bool isTopCardCovered() const override;

private:
    class Snapshot;

    bool shouldAddCards(const cards::Cards& cardsToAdd) const;
    bool shouldPullOutCards(unsigned quantity) const;
    bool isFirstCardToAddKing(const cards::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const cards::Cards& cardsToAdd) const;

    cards::Cards cards;
    unsigned topCoveredCardPosition {0};
};

class TableauPile::Snapshot: public archivers::interfaces::Snapshot {
public:
    Snapshot(std::shared_ptr<TableauPile>, cards::Cards,
             unsigned topCoveredCardPosition);

    void restore() const override;
    bool isSnapshotOfSameObject(
        const archivers::interfaces::Snapshot&) const override;

private:
    const std::shared_ptr<TableauPile> tableauPile;
    const cards::Cards pileCards;
    const unsigned topCoveredCardPosition;
};

}
