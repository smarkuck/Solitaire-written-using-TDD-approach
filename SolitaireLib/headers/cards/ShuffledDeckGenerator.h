#pragma once

#include <vector>
#include "DeckGenerator.h"

namespace solitaire::cards {

enum class Suit;
enum class Value;

class ShuffledDeckGenerator: public DeckGenerator {
public:
    Deck generate() const override;

private:
    Deck generateSortedDeck() const;

    static const std::vector<Value> values;
    static const std::vector<Suit> suits;
};

}
