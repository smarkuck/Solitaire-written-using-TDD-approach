#include "cards/Card.h"

namespace solitaire {
namespace cards {

Card::Card(Value value, Suit suit):
    value(value), suit(suit) {
}

Value Card::getValue() const {
    return value;
}

Suit Card::getSuit() const {
    return suit;
}

bool Card::hasValueOneGreaterThan(const Card& card) const {
    return static_cast<std::underlying_type<Value>::type>(value) - 1 ==
           static_cast<std::underlying_type<Value>::type>(card.value);
}

bool Card::hasSameSuitAs(const Card& card) const {
    return suit == card.getSuit();
}

bool Card::hasDifferentColorThan(const Card& card) const {
    return hasRedColor() != card.hasRedColor();
}

bool Card::hasRedColor() const {
    return suit == Suit::Heart or suit == Suit::Diamond;
}

}
}
