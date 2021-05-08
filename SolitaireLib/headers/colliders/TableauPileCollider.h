#pragma once

#include "cards/Cards.h"
#include "geometry/Position.h"
#include "interfaces/colliders/TableauPileCollider.h"

namespace solitaire::piles::interfaces {
class TableauPile;
}

namespace solitaire::colliders {

class TableauPileCollider: public interfaces::TableauPileCollider {
public:
    TableauPileCollider(const geometry::Position&,
                        const piles::interfaces::TableauPile&);

    std::optional<unsigned> tryGetCollidedCardIndex(
        const geometry::Position&) const override;

    geometry::Position getCardPosition(const unsigned index) const override;

private:
    bool isEmptyOrNotCollidesWithPileXPosition(const geometry::Position&) const;
    bool collidesWithPileXPosition(const geometry::Position&) const;
    bool collidesWithCardYPosition(const geometry::Position&,
                                   const unsigned index) const;

    geometry::Position getRelativeCardPosition(
        const unsigned index, const unsigned topCoveredCardPosition) const;

    void throwIfInvalidIndex(const unsigned index) const;

    const geometry::Position position;
    const piles::interfaces::TableauPile& tableauPile;
};

}
