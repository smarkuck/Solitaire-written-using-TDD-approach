#pragma once

#include "Deck.h"

namespace solitaire::cards {

class DeckGenerator {
public:
    virtual ~DeckGenerator() = default;
    virtual Deck generate() const = 0;
};

}
