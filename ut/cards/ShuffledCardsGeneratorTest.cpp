#include <algorithm>
#include <vector>

#include "gmock/gmock.h"
#include "cards/ShuffledCardsGenerator.h"

using namespace testing;

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

Cards createExpectedCards() {
    Cards expectedCards;
    for (const auto& suit: suits)
        for (const auto& value: values)
            expectedCards.emplace_back(value, suit);
    return expectedCards;
}

bool cardsComparator(const Card& lhs, const Card& rhs) {
    if (lhs.getSuit() == rhs.getSuit())
        return lhs.getValue() < rhs.getValue();
    return lhs.getSuit() < rhs.getSuit();
}

const Cards expectedCards = createExpectedCards();
}

class ShuffledCardsGeneratorTest: public Test {
public:
    ShuffledCardsGenerator generator;
};

TEST_F(ShuffledCardsGeneratorTest, generateShuffledCards) {
    auto generatedCards = generator.generate();

    EXPECT_FALSE(std::is_sorted(
        generatedCards.begin(), generatedCards.end(), cardsComparator));

    std::sort(generatedCards.begin(), generatedCards.end(), cardsComparator);
    EXPECT_THAT(generatedCards, ContainerEq(expectedCards));
}

TEST_F(ShuffledCardsGeneratorTest, generatedCardsHaveDifferentOrder) {
    EXPECT_NE(generator.generate(), generator.generate());
}

}
}
