#pragma once

#include "TableauPile.h"
#include "archivers/Snapshot.h"

namespace solitaire::piles {

class DefaultTableauPile: public std::enable_shared_from_this<DefaultTableauPile>,
                          public TableauPile {
public:
    void initialize(const cards::Cards::const_iterator& begin,
                    const cards::Cards::const_iterator& end) override;

    std::unique_ptr<archivers::Snapshot> createSnapshot() override;

    void tryUncoverTopCard() override;
    void tryAddCards(cards::Cards& cardsToAdd) override;
    cards::Cards tryPullOutCards(unsigned quantity) override;

    const cards::Cards& getCards() const override;
    unsigned getPlaceInOrderOfFirstCoveredCard() const override;

private:
    class Snapshot;

    bool shouldAddCards(const cards::Cards& cardsToAdd) const;
    bool shouldPullOutCards(unsigned quantity) const;
    bool isTopCardCovered() const;
    bool isFirstCardToAddKing(const cards::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const cards::Cards& cardsToAdd) const;

    cards::Cards cards;
    unsigned placeInOrderOfFirstCoveredCard {0};
};

class DefaultTableauPile::Snapshot: public archivers::Snapshot {
public:
    Snapshot(std::shared_ptr<DefaultTableauPile>, cards::Cards,
             unsigned placeInOrderOfFirstCoveredCard);

    void restore() const override;

private:
    const std::shared_ptr<DefaultTableauPile> tableauPile;
    const cards::Cards pileCards;
    const unsigned placeInOrderOfFirstCoveredCard;
};

}
