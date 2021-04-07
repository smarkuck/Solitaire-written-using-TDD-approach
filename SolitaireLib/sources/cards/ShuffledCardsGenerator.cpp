#include <algorithm>
#include <random>

#include "cards/Card.h"
#include "cards/ShuffledCardsGenerator.h"
#include "cards/Suit.h"
#include "cards/Value.h"

namespace solitaire {
namespace cards {

const std::vector<Value> ShuffledCardsGenerator::values {
    Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five,
    Value::Six, Value::Seven, Value::Eight, Value::Nine, Value::Ten,
    Value::Jack, Value::Queen, Value::King
};

const std::vector<Suit> ShuffledCardsGenerator::suits {
    Suit::Heart, Suit::Spade, Suit::Diamond, Suit::Club
};

Cards ShuffledCardsGenerator::generate() const {
    auto generatedCards = generateSortedCards();
    auto rd = std::random_device {};
    auto dre = std::default_random_engine {rd()};
    std::shuffle(std::begin(generatedCards), std::end(generatedCards), dre);
    return generatedCards;
}

Cards ShuffledCardsGenerator::generateSortedCards() const {
    Cards generatedCards;
    for (const auto& suit: suits)
        for (const auto& value: values)
            generatedCards.emplace_back(value, suit);
    return generatedCards;
}

}
}
