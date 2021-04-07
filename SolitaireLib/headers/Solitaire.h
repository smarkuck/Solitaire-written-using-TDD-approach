#pragma once

#include <array>
#include <memory>

#include "cards/CardsGenerator.h"
#include "piles/StockPile.h"
#include "piles/FoundationPile.h"
#include "piles/TableauPile.h"

namespace solitaire {

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
