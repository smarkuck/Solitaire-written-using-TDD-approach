#pragma once

#include "gmock/gmock.h"
#include "interfaces/colliders/TableauPileCollider.h"

namespace solitaire::colliders {

class TableauPileColliderMock: public interfaces::TableauPileCollider {
public:
    MOCK_METHOD(geometry::Position, getCardPosition, (const unsigned), (const));
};

}
