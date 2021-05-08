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

std::optional<unsigned>
TableauPileCollider::tryGetCollidedCardIndex(const Position& position) const {
    if (isEmptyOrNotCollidesWithPileXPosition(position))
        return std::nullopt;

    for (int cardIndex = tableauPile.getCards().size() - 1; cardIndex >= 0; --cardIndex)
        if (collidesWithCardYPosition(position, cardIndex))
            return cardIndex;

    return std::nullopt;
}

bool TableauPileCollider::
isEmptyOrNotCollidesWithPileXPosition(const Position& position) const {
    return tableauPile.getCards().empty() or not collidesWithPileXPosition(position);
}

bool TableauPileCollider::collidesWithPileXPosition(const Position& position) const {
    return position.x >= this->position.x and
           position.x < this->position.x + Layout::cardSize.width;
}

bool TableauPileCollider::collidesWithCardYPosition(
    const Position& position, const unsigned index) const
{
    const auto cardPosition = getCardPosition(index);
    return position.y >= cardPosition.y and
           position.y < cardPosition.y + Layout::cardSize.height;
}

bool TableauPileCollider::collidesWithCardsInHand(const Position& position) const {
    return std::abs(position.x - this->position.x) < Layout::cardSize.width and
           std::abs(position.y - getLastCardOrPilePosition().y) < Layout::cardSize.height;
}

Position TableauPileCollider::getLastCardOrPilePosition() const {
    const auto& cards = tableauPile.getCards();
    if (cards.empty())
        return position;
    return getCardPosition(cards.size() - 1);
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
