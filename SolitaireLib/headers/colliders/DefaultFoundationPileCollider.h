#pragma once

#include "geometry/Position.h"
#include "interfaces/colliders/FoundationPileCollider.h"

namespace solitaire::colliders {

class DefaultFoundationPileCollider: public interfaces::FoundationPileCollider {
public:
    DefaultFoundationPileCollider(const geometry::Position&);

    geometry::Position getPosition() const override;
    bool collidesWithPoint(const geometry::Position&) const override;
    bool collidesWithCard(const geometry::Position&) const override;

private:
    geometry::Position position;
};

}
