#pragma once

#include "archivers/Archiver.h"
#include "cards/Cards.h"
#include "cards/Deck.h"

namespace solitaire::piles {

class TableauPile: public archivers::Archiver {
public:
    virtual ~TableauPile() = default;

    virtual void initialize(const cards::Deck::const_iterator& begin,
                            const cards::Deck::const_iterator& end) = 0;

    virtual void tryUncoverTopCard() = 0;
    virtual void tryAddCards(cards::Cards& cardsToAdd) = 0;
    virtual cards::Cards tryPullOutCards(unsigned quantity) = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual unsigned getPlaceInOrderOfFirstCoveredCard() const = 0;
};

}
