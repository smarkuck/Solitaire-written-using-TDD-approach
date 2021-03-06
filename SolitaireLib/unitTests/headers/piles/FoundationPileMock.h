#pragma once

#include "gmock/gmock.h"
#include "interfaces/piles/FoundationPile.h"

namespace solitaire::piles {

class FoundationPileMock: public interfaces::FoundationPile {
public:
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(std::unique_ptr<archivers::interfaces::Snapshot>,
                createSnapshot, (), (override));
    MOCK_METHOD(void, tryAddCard, (std::optional<cards::Card>&), (override));
    MOCK_METHOD(std::optional<cards::Card>, tryPullOutCard, (), (override));
    MOCK_METHOD(const cards::Cards&, getCards, (), (const, override));
    MOCK_METHOD(std::optional<cards::Value>, getTopCardValue, (), (const, override));
};

}
