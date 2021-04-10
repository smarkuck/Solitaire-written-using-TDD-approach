#pragma once

#include <optional>

#include "archivers/Archiver.h"
#include "cards/Cards.h"
#include "cards/Deck.h"

namespace solitaire::piles {

class StockPile: public archivers::Archiver {
public:
    virtual ~StockPile() = default;

    virtual void initialize(const cards::Deck::const_iterator& begin,
                            const cards::Deck::const_iterator& end) = 0;

    virtual void selectNextCard() = 0;
    virtual std::optional<cards::Card> tryPullOutCard() = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual std::optional<unsigned> getSelectedCardIndex() const = 0;
};

}
