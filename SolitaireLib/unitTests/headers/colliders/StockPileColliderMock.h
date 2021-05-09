#pragma once

#include "geometry/Position.h"
#include "gmock/gmock.h"
#include "interfaces/colliders/StockPileCollider.h"

namespace solitaire::colliders {

class StockPileColliderMock: public interfaces::StockPileCollider {
public:
    MOCK_METHOD(bool, coveredCardsCollidesWith,
                (const geometry::Position&), (const, override));
    MOCK_METHOD(bool, uncoveredCardsCollidesWith,
                (const geometry::Position&), (const, override));
    MOCK_METHOD(geometry::Position, getCoveredCardsPosition, (), (const, override));
    MOCK_METHOD(geometry::Position, getUncoveredCardsPosition, (), (const, override));
};

}
