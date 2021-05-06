#pragma once

#include "colliders/FoundationPileCollider.h"
#include "geometry/Position.h"

namespace solitaire::colliders {

class DefaultFoundationPileCollider: public FoundationPileCollider {
public:
    DefaultFoundationPileCollider(const geometry::Position&);

    geometry::Position getPosition() const override;

private:
    geometry::Position position;
};

}
