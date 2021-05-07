#pragma once

#include <vector>
#include "interfaces/cards/DeckGenerator.h"

namespace solitaire::cards {

enum class Suit;
enum class Value;

class ShuffledDeckGenerator: public interfaces::DeckGenerator {
public:
    Deck generate() const override;

private:
    Deck generateSortedDeck() const;

    static const std::vector<Value> values;
    static const std::vector<Suit> suits;
};

}
