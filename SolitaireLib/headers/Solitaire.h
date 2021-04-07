#pragma once

#include <array>
#include <memory>

#include "cards/Cards.h"

namespace solitaire {
namespace cards {
    class CardsGenerator;
}

namespace piles {
    class FoundationPile;
    class StockPile;
    class TableauPile;
}

class Solitaire {
    using FoundationPiles = std::array<std::unique_ptr<piles::FoundationPile>, 4>;
    using TableauPiles = std::array<std::unique_ptr<piles::TableauPile>, 7>;

public:
    Solitaire(std::unique_ptr<cards::CardsGenerator> cardsGenerator,
              std::unique_ptr<piles::StockPile> stockPile,
              FoundationPiles foundationPiles,
              TableauPiles tableauPiles);

    void startNewGame();

private:
    cards::Cards::const_iterator
    initializeTableauPilesAndReturnFirstNotUsedCard(const cards::Cards&);

    std::unique_ptr<cards::CardsGenerator> cardsGenerator;
    std::unique_ptr<piles::StockPile> stockPile;
    FoundationPiles foundationPiles;
    TableauPiles tableauPiles;
};

}
