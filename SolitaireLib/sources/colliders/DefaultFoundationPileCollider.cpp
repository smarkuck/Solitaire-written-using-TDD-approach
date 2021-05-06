#include "Layout.h"
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

bool DefaultFoundationPileCollider::collidesWithPoint(
    const geometry::Position& position) const
{
    return position.x >= this->position.x and
           position.x < this->position.x + Layout::cardSize.width and
           position.y >= this->position.y and
           position.y < this->position.y + Layout::cardSize.height;
}

bool DefaultFoundationPileCollider::collidesWithCard(
    const geometry::Position& position) const
{
    return std::abs(position.x - this->position.x) < Layout::cardSize.width and
           std::abs(position.y - this->position.y) < Layout::cardSize.height;
}

}
