#include <algorithm>
#include <random>

#include "cards/Card.h"
#include "cards/ShuffledDeckGenerator.h"
#include "cards/Suit.h"
#include "cards/Value.h"

namespace solitaire::cards {

const std::vector<Value> ShuffledDeckGenerator::values {
    Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five,
    Value::Six, Value::Seven, Value::Eight, Value::Nine, Value::Ten,
    Value::Jack, Value::Queen, Value::King
};

const std::vector<Suit> ShuffledDeckGenerator::suits {
    Suit::Heart, Suit::Spade, Suit::Diamond, Suit::Club
};

Deck ShuffledDeckGenerator::generate() const {
    auto generatedDeck = generateSortedDeck();
    auto rd = std::random_device {};
    auto dre = std::default_random_engine {rd()};
    std::shuffle(std::begin(generatedDeck), std::end(generatedDeck), dre);
    return generatedDeck;
}

Deck ShuffledDeckGenerator::generateSortedDeck() const {
    Deck generatedDeck;
    unsigned index = 0;
    for (const auto& suit: suits)
        for (const auto& value: values)
            generatedDeck[index++] = Card {value, suit};
    return generatedDeck;
}

}
