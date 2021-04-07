#pragma once

#include "piles/TableauPile.h"

namespace solitaire {
namespace piles {

class TableauPileMock: public TableauPile {
public:
    MOCK_METHOD(void, initialize, (const cards::Cards::const_iterator&,
                                   const cards::Cards::const_iterator&), (override));
    MOCK_METHOD(void, tryUncoverTopCard, (), (override));
    MOCK_METHOD(void, tryAddCards, (cards::Cards&), (override));
    MOCK_METHOD(cards::Cards, tryPullOutCards, (unsigned), (override));
    MOCK_METHOD(void, tryRestoreLastPulledOutCards, (), (override));
    MOCK_METHOD(const cards::Cards&, getCards, (), (const, override));
    MOCK_METHOD(unsigned, getPlaceInOrderOfFirstCoveredCard, (), (const, override));
};

}
}
