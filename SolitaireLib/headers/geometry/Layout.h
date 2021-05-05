#pragma once

#include "Position.h"
#include "Size.h"

namespace solitaire::geometry {

struct Layout {
    static constexpr int firstFoundationPilePositionX {283};
    static constexpr int foundationPilePositionY {30};
    static constexpr int firstTableauPilePositionX {16};
    static constexpr int tableauPilePositionY {144};
    static constexpr int pilesSpacing {89};
    static constexpr int coveredTableauPileCardsSpacing {3};
    static constexpr int uncoveredTableauPileCardsSpacing {16};
    static constexpr int stockPileCardsSpacing {2};

    static constexpr Position stockPilePosition {16, 30};
    static constexpr Size cardSize {75, 104};
};

}
