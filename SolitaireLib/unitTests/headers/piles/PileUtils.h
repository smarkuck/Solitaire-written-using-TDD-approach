#pragma once

#include <algorithm>

#include "cards/Cards.h"
#include "cards/Deck.h"

namespace solitaire::piles {

template <class T>
void initializePile(T& pile, const cards::Cards& cards) {
    cards::Deck deck;
    std::copy_n(cards.begin(), cards.size(), deck.begin());
    pile.initialize(deck.begin(), std::next(deck.begin(), cards.size()));
}

}
