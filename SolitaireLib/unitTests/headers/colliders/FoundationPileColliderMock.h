#pragma once

#include "colliders/FoundationPileCollider.h"
#include "gmock/gmock.h"

namespace solitaire::colliders {

class FoundationPileColliderMock: public FoundationPileCollider {
public:
    MOCK_METHOD(geometry::Position, getPosition, (), (const, override));
};

}
