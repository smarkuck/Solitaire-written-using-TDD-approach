#pragma once

#include "geometry/Position.h"
#include "interfaces/colliders/FoundationPileCollider.h"

namespace solitaire::colliders {

class FoundationPileCollider: public interfaces::FoundationPileCollider {
public:
    FoundationPileCollider(const geometry::Position&);

    bool collidesWithPoint(const geometry::Position&) const override;
    bool collidesWithCardsInHand(const geometry::Position&) const override;
    geometry::Position getPosition() const override;

private:
    const geometry::Position position;
};

}
