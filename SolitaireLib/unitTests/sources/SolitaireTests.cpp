#include <algorithm>

#include "mock_ptr.h"
#include "Solitaire.h"
#include "archivers/Snapshot.h"
#include "cards/Card.h"
#include "cards/CardsGeneratorMock.h"
#include "cards/CardsGeneratorUtils.h"
#include "gmock/gmock.h"
#include "piles/FoundationPileMock.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

namespace {
    const auto expectedCards {createSortedCards()};
}

MATCHER_P2(RangeEq, begin, end, "") {
    return std::equal(std::get<0>(arg), std::get<1>(arg), begin, end);
}

class SolitaireTest: public Test {
public:
    template <class T, std::size_t N>
    using SharedPtrArray = std::array<std::shared_ptr<T>, N>;

    template <class T, std::size_t N>
    SharedPtrArray<T, N> makeSharedPtrArray() {
        SharedPtrArray<T, N> result;
        for (auto& ptr: result)
            ptr = std::make_shared<T>();
        return result;
    }

    template <class CopiedArrayType, class T, std::size_t N>
    SharedPtrArray<CopiedArrayType, N> copySharedPtrArray(const SharedPtrArray<T, N>& source) {
        SharedPtrArray<CopiedArrayType, N> copy;
        std::copy(source.begin(), source.end(), copy.begin());
        return copy;
    }

    mock_ptr<CardsGeneratorMock> cardsGeneratorMock;
    std::shared_ptr<StockPileMock> stockPileMock {std::make_shared<StockPileMock>()};

    std::array<std::shared_ptr<FoundationPileMock>, 4>
        foundationPileMocks {makeSharedPtrArray<FoundationPileMock, 4>()};

    std::array<std::shared_ptr<TableauPileMock>, 7>
        tableauPileMocks {makeSharedPtrArray<TableauPileMock, 7>()};

    Solitaire solitaire {
        cardsGeneratorMock.make_unique(),
        stockPileMock,
        copySharedPtrArray<FoundationPile>(foundationPileMocks),
        copySharedPtrArray<TableauPile>(tableauPileMocks)
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
    EXPECT_CALL(*cardsGeneratorMock, generate()).WillOnce(Return(expectedCards));

    for (auto& pile: foundationPileMocks)
        EXPECT_CALL(*pile, initialize());

    expectTableauPilesInitialization(expectedCards);

    EXPECT_CALL(*stockPileMock, initialize(_, _))
        .With(AllArgs(RangeEq(std::next(expectedCards.begin(), 28), expectedCards.end())));

    solitaire.startNewGame();
}

TEST_F(SolitaireTest, throwExceptionWhenTryingToAccessFoundationPileWithIdGreaterThanThree) {
    EXPECT_THROW(solitaire.getFoundationPile(4), std::runtime_error);
}

TEST_F(SolitaireTest, getFoundationPile) {
    EXPECT_EQ(&solitaire.getFoundationPile(3), foundationPileMocks[3].get());
}

TEST_F(SolitaireTest, throwExceptionWhenTryingToAccessTableauPileWithIdGreaterThanSix) {
    EXPECT_THROW(solitaire.getTableauPile(7), std::runtime_error);
}

TEST_F(SolitaireTest, getTableauPile) {
    EXPECT_EQ(&solitaire.getTableauPile(6), tableauPileMocks[6].get());
}

TEST_F(SolitaireTest, getStockPile) {
    EXPECT_EQ(&solitaire.getStockPile(), stockPileMock.get());
}

}
