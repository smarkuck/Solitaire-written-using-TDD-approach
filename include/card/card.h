#include <ostream>
#include <type_traits>

#include "value.h"
#include "suit.h"

namespace solitaire {
namespace card {

class Card {
public:
    Card(Value value, Suit suit):
        value(value), suit(suit) {
    }

    Value getValue() const {
        return value;
    }

    Suit getSuit() const {
        return suit;
    }

    bool hasValueOneGreaterThan(const Card& card) const {
        return static_cast<std::underlying_type<Value>::type>(this->value) ==
               static_cast<std::underlying_type<Value>::type>(card.value) + 1;
    }

    bool hasDifferentColorThan(const Card& card) const {
        return hasRedColor() != card.hasRedColor();
    }

    bool hasRedColor() const {
        return suit == card::Suit::Heart or suit == card::Suit::Diamond;
    }

private:
    Value value;
    Suit suit;
};

inline bool operator==(const Card& lhs, const Card& rhs) {
    return (lhs.getValue() == rhs.getValue()) and (lhs.getSuit() == rhs.getSuit());
}

inline std::ostream& operator<<(std::ostream& os, const Card& card)
{
    return os << "Card {value: " << to_string(card.getValue())
              << ", suit: " << to_string(card.getSuit()) << '}';
}

}
}
