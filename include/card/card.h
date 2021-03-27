#include "value.h"
#include "color.h"

namespace solitaire {
namespace card {

struct Card {
    Value value;
    Color color;
};

inline bool operator==(const Card& lhs, const Card& rhs) {
    return (lhs.value == rhs.value) and (lhs.color == rhs.color);
}

inline std::ostream& operator<<(std::ostream& os, const Card& card)
{
    os << "Card {value: " << to_string(card.value) << ", color: " << to_string(card.color) << '}';
    return os;
}

}
}
