#pragma once

#include "gmock/gmock.h"
#include "interfaces/colliders/FoundationPileCollider.h"

namespace solitaire::colliders {

class FoundationPileColliderMock: public interfaces::FoundationPileCollider {
public:
    MOCK_METHOD(geometry::Position, getPosition, (), (const, override));
    MOCK_METHOD(bool, collidesWithPoint,
                (const geometry::Position&), (const, override));
    MOCK_METHOD(bool, collidesWithCard,
                (const geometry::Position&), (const, override));
};

}
