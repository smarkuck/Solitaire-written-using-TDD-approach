#pragma once

#include "TexturePosition.h"
#include "TextureSize.h"

namespace solitaire::graphics {

class Layout {
public:
    static constexpr unsigned pilesSpacing {89};
    static constexpr unsigned firstFoundationPilePositionX {283};
    static constexpr unsigned foundationPilePositionY {30};
    static constexpr unsigned firstTableauPilePositionX {16};
    static constexpr unsigned tableauPilePositionY {144};
    static constexpr unsigned coveredTableauPileCardsSpacing {3};
    static constexpr unsigned uncoveredTableauPileCardsSpacing {16};
    static constexpr unsigned stockPileCardsSpacing {2};

    static constexpr TexturePosition stockPilePosition {16, 30};
    static constexpr TextureSize cardSize {75, 104};
};

}
