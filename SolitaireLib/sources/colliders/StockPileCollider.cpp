#include "Layout.h"
#include "cards/Card.h"
#include "colliders/StockPileCollider.h"
#include "geometry/Position.h"
#include "piles/StockPile.h"

using namespace solitaire::piles::interfaces;
using namespace solitaire::geometry;

namespace solitaire::colliders {

StockPileCollider::StockPileCollider(const StockPile& stockPile):
    stockPile {stockPile} {
}

bool StockPileCollider::coveredCardsCollidesWith(const Position& position) const {
    const auto cardsPosition = getCoveredCardsPosition();
    return cardCollidesWith(cardsPosition, position);
}

bool StockPileCollider::uncoveredCardsCollidesWith(const Position& position) const {
    const auto cardsPosition = getUncoveredCardsPosition();
    return cardCollidesWith(cardsPosition, position);
}

Position StockPileCollider::getCoveredCardsPosition() const {
    const auto howManyTimesMoveCards {
        getHowManyTimesMoveCards(getCoveredCardsCount())};

    return Layout::stockPilePosition +
           Position {Layout::stockPileCardsSpacing * howManyTimesMoveCards, 0};
}

Position StockPileCollider::getUncoveredCardsPosition() const {
    const auto howManyTimesMoveCards {getHowManyTimesMoveCards(getUncoveredCardsCount())};
    const auto moveDistance = Layout::stockPileCardsSpacing * howManyTimesMoveCards;

    return Layout::stockPilePosition +
        Position {Layout::pilesSpacing + moveDistance, 0};
}

int StockPileCollider::getHowManyTimesMoveCards(const int cardsCount) const {
    return (cardsCount - 1) / 6;
}

int StockPileCollider::getCoveredCardsCount() const {
    const auto selectedCardIndex = stockPile.getSelectedCardIndex();
    const auto& cards = stockPile.getCards();

    if (selectedCardIndex)
        return cards.size() - (selectedCardIndex.value() + 1);
    return cards.size();
}

int StockPileCollider::getUncoveredCardsCount() const {
    const auto selectedCardIndex = stockPile.getSelectedCardIndex();
    return selectedCardIndex ? selectedCardIndex.value() + 1 : 0;
}

bool StockPileCollider::cardCollidesWith(const Position& cardPosition,
                                         const Position& point) const
{
    return point.x >= cardPosition.x and
           point.x < cardPosition.x + Layout::cardSize.width and
           point.y >= cardPosition.y and
           point.y < cardPosition.y + Layout::cardSize.height;
}

}
