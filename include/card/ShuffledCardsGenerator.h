#include <vector>

#include "Cards.h"

namespace solitaire {
namespace card {

class ShuffledCardsGenerator {
public:
    Cards generate() const;

private:
    Cards generateSortedCards() const;

    static const std::vector<Value> values;
    static const std::vector<Suit> suits;
};

}
}
