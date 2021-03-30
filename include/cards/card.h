#include <ostream>
#include <type_traits>

#include "Value.h"
#include "Suit.h"

namespace solitaire {
namespace cards {

class Card {
public:
    Card(Value value, Suit suit);

    Value getValue() const;
    Suit getSuit() const;

    bool hasValueOneGreaterThan(const Card& card) const;
    bool hasSameSuitAs(const Card& card) const;
    bool hasDifferentColorThan(const Card& card) const;
    bool hasRedColor() const;

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
