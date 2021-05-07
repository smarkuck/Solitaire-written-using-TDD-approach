#pragma once

#include "geometry/Position.h"
#include "interfaces/colliders/FoundationPileCollider.h"

namespace solitaire::colliders {

class FoundationPileCollider: public interfaces::FoundationPileCollider {
public:
    FoundationPileCollider(const geometry::Position&);

    geometry::Position getPosition() const override;
    bool collidesWithPoint(const geometry::Position&) const override;
    bool collidesWithCard(const geometry::Position&) const override;

private:
    const geometry::Position position;
};

}
