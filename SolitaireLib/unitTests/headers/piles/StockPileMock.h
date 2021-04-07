#pragma once

#include "gmock/gmock.h"
#include "piles/StockPile.h"

namespace solitaire {
namespace piles {

class StockPileMock: public StockPile {
public:
    MOCK_METHOD(void, initialize, (const cards::Cards::const_iterator&,
                                   const cards::Cards::const_iterator&), (override));
    MOCK_METHOD(void, selectNextCard, (), (override));
    MOCK_METHOD(std::optional<cards::Card>, tryPullOutCard, (), (override));
    MOCK_METHOD(void, tryRestoreLastPulledOutCard, (), (override));
    MOCK_METHOD(const cards::Cards&, getCards, (), (const, override));
    MOCK_METHOD(std::optional<unsigned>, getSelectedCardIndex, (), (const, override));
};

}
}
