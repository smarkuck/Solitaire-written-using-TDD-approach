#pragma once

#include <optional>

#include "cards/Cards.h"
#include "cards/Deck.h"
#include "interfaces/archivers/Archiver.h"

namespace solitaire::piles::interfaces {

class StockPile: public archivers::interfaces::Archiver {
public:
    virtual ~StockPile() = default;

    virtual void initialize(const cards::Deck::const_iterator& begin,
                            const cards::Deck::const_iterator& end) = 0;

    virtual void trySelectNextCard() = 0;
    virtual std::optional<cards::Card> tryPullOutCard() = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual std::optional<unsigned> getSelectedCardIndex() const = 0;
};

}
