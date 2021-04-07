#include <algorithm>

#include "gmock/gmock.h"
#include "mock_ptr_array.h"
#include "Solitaire.h"
#include "cards/CardsGeneratorMock.h"
#include "cards/CardsGeneratorUtils.h"
#include "piles/FoundationPileMock.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

MATCHER_P2(RangeEq, begin, end, "") {
    return std::equal(std::get<0>(arg), std::get<1>(arg), begin, end);
}

class SolitaireTest: public Test {
public:
    mock_ptr<CardsGeneratorMock> cardsGeneratorMock;
    mock_ptr<StockPileMock> stockPileMock;
    mock_ptr_array<FoundationPileMock, 4> foundationPileMocks;
    mock_ptr_array<TableauPileMock, 7> tableauPileMocks;

    Solitaire solitaire {
        cardsGeneratorMock.make_unique(),
        stockPileMock.make_unique(),
        foundationPileMocks.make_unique<FoundationPile>(),
        tableauPileMocks.make_unique<TableauPile>()
    };

    void expectTableauPilesInitialization(const Cards& cards) {
        EXPECT_CALL(*tableauPileMocks[0], initialize(_, _))
            .With(AllArgs(RangeEq(cards.begin(), std::next(cards.begin()))));
        EXPECT_CALL(*tableauPileMocks[1], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin()), std::next(cards.begin(), 3))));
        EXPECT_CALL(*tableauPileMocks[2], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin(), 3), std::next(cards.begin(), 6))));
        EXPECT_CALL(*tableauPileMocks[3], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin(), 6), std::next(cards.begin(), 10))));
        EXPECT_CALL(*tableauPileMocks[4], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin(), 10), std::next(cards.begin(), 15))));
        EXPECT_CALL(*tableauPileMocks[5], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin(), 15), std::next(cards.begin(), 21))));
        EXPECT_CALL(*tableauPileMocks[6], initialize(_, _))
            .With(AllArgs(RangeEq(std::next(cards.begin(), 21), std::next(cards.begin(), 28))));
    }
};

TEST_F(SolitaireTest, onNewGameGenerateAndDistributeCards) {
    const auto cards = createSortedCards();

    EXPECT_CALL(*cardsGeneratorMock, generate()).WillOnce(Return(cards));

    for (auto& pile: foundationPileMocks)
        EXPECT_CALL(*pile, initialize());

    expectTableauPilesInitialization(cards);

    EXPECT_CALL(*stockPileMock, initialize(_, _))
        .With(AllArgs(RangeEq(std::next(cards.begin(), 28), cards.end())));

    solitaire.startNewGame();
}

}
