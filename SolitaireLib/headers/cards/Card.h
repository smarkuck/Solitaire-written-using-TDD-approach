#pragma once

#include <ostream>

namespace solitaire {
namespace cards {

enum class Value;
enum class Suit;

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

bool operator==(const Card& lhs, const Card& rhs);
std::ostream& operator<<(std::ostream& os, const Card& card);

}
}
