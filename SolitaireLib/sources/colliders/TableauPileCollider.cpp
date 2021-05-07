#include "Layout.h"
#include "cards/Card.h"
#include "colliders/TableauPileCollider.h"
#include "piles/TableauPile.h"

using namespace solitaire::cards;
using namespace solitaire::geometry;
using namespace solitaire::piles::interfaces;

namespace solitaire::colliders {

TableauPileCollider::TableauPileCollider(
    const Position& position, const TableauPile& tableauPile):
        position {std::move(position)}, tableauPile {std::move(tableauPile)} {
}

Position TableauPileCollider::getCardPosition(const unsigned index) const {
    throwIfInvalidIndex(index);
    const auto topCoveredCardPosition = tableauPile.getTopCoveredCardPosition();
    return position + getRelativeCardPosition(index, topCoveredCardPosition);
}

void TableauPileCollider::throwIfInvalidIndex(const unsigned index) const {
    if (index >= tableauPile.getCards().size())
        throw std::runtime_error {"Invalid tableau pile card index: " + index};
}

Position TableauPileCollider::getRelativeCardPosition(
    const unsigned index, const unsigned topCoveredCardPosition) const
{
    int y = 0;
    for (unsigned i = 0; i < topCoveredCardPosition and i < index; ++i)
        y += Layout::coveredTableauPileCardsSpacing;
    for (unsigned i = topCoveredCardPosition; i < index; ++i)
        y += Layout::uncoveredTableauPileCardsSpacing;
    return Position {0, y};
}

}
