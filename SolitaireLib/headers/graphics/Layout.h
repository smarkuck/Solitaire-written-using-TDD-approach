#pragma once

#include "TexturePosition.h"
#include "TextureSize.h"

namespace solitaire::graphics {

struct Layout {
    static constexpr int firstFoundationPilePositionX {283};
    static constexpr int foundationPilePositionY {30};
    static constexpr int firstTableauPilePositionX {16};
    static constexpr int tableauPilePositionY {144};
    static constexpr int pilesSpacing {89};
    static constexpr int coveredTableauPileCardsSpacing {3};
    static constexpr int uncoveredTableauPileCardsSpacing {16};
    static constexpr int stockPileCardsSpacing {2};

    static constexpr TexturePosition stockPilePosition {16, 30};
    static constexpr TextureSize cardSize {75, 104};
};

}
