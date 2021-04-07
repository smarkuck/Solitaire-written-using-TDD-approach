#pragma once

#include <vector>
#include "CardsGenerator.h"

namespace solitaire {
namespace cards {

enum class Suit;
enum class Value;

class ShuffledCardsGenerator: public CardsGenerator {
public:
    Cards generate() const override;

private:
    Cards generateSortedCards() const;

    static const std::vector<Value> values;
    static const std::vector<Suit> suits;
};

}
}
