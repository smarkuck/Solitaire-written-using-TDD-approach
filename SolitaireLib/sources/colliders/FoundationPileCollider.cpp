#include "Layout.h"
#include "colliders/FoundationPileCollider.h"

using namespace solitaire::geometry;

namespace solitaire::colliders {

FoundationPileCollider::FoundationPileCollider(
    const Position& position):
        position {position} {
}

Position FoundationPileCollider::getPosition() const {
    return position;
}

bool FoundationPileCollider::collidesWithPoint(
    const geometry::Position& position) const
{
    return position.x >= this->position.x and
           position.x < this->position.x + Layout::cardSize.width and
           position.y >= this->position.y and
           position.y < this->position.y + Layout::cardSize.height;
}

bool FoundationPileCollider::collidesWithCard(
    const geometry::Position& position) const
{
    return std::abs(position.x - this->position.x) < Layout::cardSize.width and
           std::abs(position.y - this->position.y) < Layout::cardSize.height;
}

}
