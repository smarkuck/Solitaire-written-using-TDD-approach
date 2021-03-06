#pragma once

#include "gmock/gmock.h"
#include "interfaces/colliders/FoundationPileCollider.h"

namespace solitaire::colliders {

class FoundationPileColliderMock: public interfaces::FoundationPileCollider {
public:
    MOCK_METHOD(bool, collidesWithPoint,
                (const geometry::Position&), (const, override));
    MOCK_METHOD(bool, collidesWithCardsInHand,
                (const geometry::Position&), (const, override));
    MOCK_METHOD(geometry::Position, getPosition, (), (const, override));
};

}
