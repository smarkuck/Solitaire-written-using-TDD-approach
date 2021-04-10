#include "cards/CardsGeneratorUtils.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"

namespace solitaire::cards {

namespace {
std::vector<Value> createValues() {
    return std::vector<Value> {
        Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five,
        Value::Six, Value::Seven, Value::Eight, Value::Nine, Value::Ten,
        Value::Jack, Value::Queen, Value::King
    };
}

std::vector<Suit> createSuits() {
    return std::vector<Suit> {
        Suit::Heart, Suit::Spade, Suit::Diamond, Suit::Club
    };
}
}

Cards createSortedCards() {
    static const auto values {createValues()};
    static const auto suits {createSuits()};

    Cards expectedCards;
    for (const auto& suit: suits)
        for (const auto& value: values)
            expectedCards.emplace_back(value, suit);
    return expectedCards;
}

}
