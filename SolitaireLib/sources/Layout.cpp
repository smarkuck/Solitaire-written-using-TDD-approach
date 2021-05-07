#include "Layout.h"
#include "interfaces/Solitaire.h"
#include "piles/PileId.h"

using namespace solitaire::interfaces;
using namespace solitaire::piles;
using namespace solitaire::geometry;

namespace solitaire {

Position Layout::getFoundationPilePosition(const piles::PileId id) {
    if (id >= Solitaire::foundationPilesCount)
        throw std::runtime_error {"Invalid foundation pile id: " + id};

    return Position {
        firstFoundationPilePositionX + static_cast<int>(id) * pilesSpacing,
        foundationPilePositionY
    };
}

Position Layout::getTableauPilePosition(const piles::PileId id) {
    if (id >= Solitaire::tableauPilesCount)
        throw std::runtime_error {"Invalid tableau pile id: " + id};

    return Position {
        firstTableauPilePositionX + static_cast<int>(id) * pilesSpacing,
        tableauPilePositionY
    };
}

}
