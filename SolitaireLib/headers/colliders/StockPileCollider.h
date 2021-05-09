#pragma once

#include "interfaces/colliders/StockPileCollider.h"

namespace solitaire::piles::interfaces {
class StockPile;
}

namespace solitaire::colliders {

class StockPileCollider: public interfaces::StockPileCollider {
public:
    StockPileCollider(const piles::interfaces::StockPile&);

    bool coveredCardsCollidesWith(const geometry::Position&) const override;
    bool uncoveredCardsCollidesWith(const geometry::Position&) const override;
    geometry::Position getCoveredCardsPosition() const override;
    geometry::Position getUncoveredCardsPosition() const override;

private:
    int getHowManyTimesMoveCards(const int cardsCount) const;
    int getCoveredCardsCount() const;
    int getUncoveredCardsCount() const;
    bool cardCollidesWith(const geometry::Position& cardPosition,
                          const geometry::Position& point) const;

    const piles::interfaces::StockPile& stockPile;
};

}
