#pragma once

#include "cards/Deck.h"

namespace solitaire::cards::interfaces {

class DeckGenerator {
public:
    virtual ~DeckGenerator() = default;
    virtual Deck generate() const = 0;
};

}
