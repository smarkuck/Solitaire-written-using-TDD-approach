#include <algorithm>

#include "cards/Card.h"
#include "cards/DeckGeneratorUtils.h"
#include "cards/ShuffledDeckGenerator.h"
#include "gmock/gmock.h"

using namespace testing;

namespace solitaire::cards {

namespace {
bool cardsComparator(const Card& lhs, const Card& rhs) {
    if (lhs.getSuit() == rhs.getSuit())
        return lhs.getValue() < rhs.getValue();
    return lhs.getSuit() < rhs.getSuit();
}
}

class ShuffledDeckGeneratorTest: public Test {
public:
    ShuffledDeckGenerator generator;
};

TEST_F(ShuffledDeckGeneratorTest, generateShuffledDeck) {
    auto generatedDeck = generator.generate();

    EXPECT_FALSE(std::is_sorted(
        generatedDeck.begin(), generatedDeck.end(), cardsComparator
    ));

    std::sort(generatedDeck.begin(), generatedDeck.end(), cardsComparator);
    EXPECT_THAT(generatedDeck, ContainerEq(createSortedDeck()));
}

TEST_F(ShuffledDeckGeneratorTest, generatedDecksHaveDifferentOrder) {
    EXPECT_NE(generator.generate(), generator.generate());
}

}
