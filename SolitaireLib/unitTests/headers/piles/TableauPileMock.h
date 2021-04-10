#pragma once

#include "gmock/gmock.h"
#include "piles/TableauPile.h"

namespace solitaire::piles {

class TableauPileMock: public TableauPile {
public:
    MOCK_METHOD(void, initialize, (const cards::Deck::const_iterator&,
                                   const cards::Deck::const_iterator&), (override));
    MOCK_METHOD(std::unique_ptr<archivers::Snapshot>, createSnapshot, (), (override));
    MOCK_METHOD(void, tryUncoverTopCard, (), (override));
    MOCK_METHOD(void, tryAddCards, (cards::Cards&), (override));
    MOCK_METHOD(cards::Cards, tryPullOutCards, (unsigned), (override));
    MOCK_METHOD(const cards::Cards&, getCards, (), (const, override));
    MOCK_METHOD(unsigned, getPlaceInOrderOfFirstCoveredCard, (), (const, override));
};

}
