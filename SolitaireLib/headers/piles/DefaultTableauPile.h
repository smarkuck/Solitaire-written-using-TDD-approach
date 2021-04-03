#pragma once

#include "TableauPile.h"

namespace solitaire {
namespace piles {

class DefaultTableauPile: public TableauPile {
public:
    void initialize(const cards::Cards::const_iterator& begin,
                    const cards::Cards::const_iterator& end) override;

    void tryUncoverTopCard() override;
    void tryAddCards(cards::Cards& cardsToAdd) override;
    cards::Cards tryPullOutCards(unsigned quantity) override;
    void tryRestoreLastPulledOutCards() override;

    const cards::Cards& getCards() const override;
    unsigned getPlaceInOrderOfFirstCoveredCard() const override;

private:
    bool shouldAddCards(const cards::Cards& cardsToAdd) const;
    bool shouldPullOutCards(unsigned quantity) const;
    bool isTopCardCovered() const;
    bool isFirstCardToAddKing(const cards::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const cards::Cards& cardsToAdd) const;

    cards::Cards cards;
    cards::Cards lastPulledOutCards;
    unsigned placeInOrderOfFirstCoveredCard {0};
};

}
}
