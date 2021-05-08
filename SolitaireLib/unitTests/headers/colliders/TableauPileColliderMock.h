#pragma once

#include "gmock/gmock.h"
#include "interfaces/colliders/TableauPileCollider.h"

namespace solitaire::colliders {

class TableauPileColliderMock: public interfaces::TableauPileCollider {
public:
    MOCK_METHOD(std::optional<unsigned>, tryGetCollidedCardIndex,
                (const geometry::Position&), (const, override));

    MOCK_METHOD(bool, collidesWithCardsInHand,
                (const geometry::Position&), (const, override));

    MOCK_METHOD(geometry::Position, getCardPosition, (const unsigned), (const));
};

}
