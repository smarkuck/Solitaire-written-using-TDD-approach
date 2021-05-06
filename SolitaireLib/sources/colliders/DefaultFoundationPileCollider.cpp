#include "colliders/DefaultFoundationPileCollider.h"

using namespace solitaire::geometry;

namespace solitaire::colliders {

DefaultFoundationPileCollider::DefaultFoundationPileCollider(
    const Position& position):
        position {position} {
}

Position DefaultFoundationPileCollider::getPosition() const {
    return position;
}

}
