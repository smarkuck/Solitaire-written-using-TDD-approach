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

    geometry::Position getCardPosition(const unsigned index) const override;

private:
    void throwIfInvalidIndex(const unsigned index) const;

    geometry::Position getRelativeCardPosition(
        const unsigned index, const unsigned topCoveredCardPosition) const;

    const geometry::Position position;
    const piles::interfaces::TableauPile& tableauPile;
};

}
