#pragma once

#include "gmock/gmock.h"
#include "piles/FoundationPile.h"

namespace solitaire {
namespace piles {

class FoundationPileMock: public FoundationPile {
public:
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(void, tryAddCard, (std::optional<cards::Card>&), (override));
    MOCK_METHOD(std::optional<cards::Card>, tryPullOutCard, (), (override));
    MOCK_METHOD(void, tryRestoreLastPulledOutCard, (), (override));
    MOCK_METHOD(const cards::Cards&, getCards, (), (const, override));
    MOCK_METHOD(std::optional<cards::Value>, getTopCardValue, (), (const, override));
};

}
}
