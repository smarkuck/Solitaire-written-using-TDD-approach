#include "cards/CardsGeneratorUtils.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"

namespace solitaire {
namespace cards {

namespace {
const std::vector<Value> values {
    Value::Ace, Value::Two, Value::Three, Value::Four, Value::Five,
    Value::Six, Value::Seven, Value::Eight, Value::Nine, Value::Ten,
    Value::Jack, Value::Queen, Value::King
};

const std::vector<Suit> suits {
    Suit::Heart, Suit::Spade, Suit::Diamond, Suit::Club
};
}

Cards createSortedCards() {
    Cards expectedCards;
    for (const auto& suit: suits)
        for (const auto& value: values)
            expectedCards.emplace_back(value, suit);
    return expectedCards;
}

}
}
