#pragma once

#include <array>
#include <memory>

#include "cards/Card.h"
#include "cards/Deck.h"

namespace solitaire {
namespace cards {
    class DeckGenerator;
}

namespace piles {
    class FoundationPile;
    class StockPile;
    class TableauPile;
}

class Solitaire {
private:
    using FoundationPiles = std::array<std::shared_ptr<piles::FoundationPile>, 4>;
    using TableauPiles = std::array<std::shared_ptr<piles::TableauPile>, 7>;

public:
    Solitaire(std::unique_ptr<cards::DeckGenerator> deckGenerator,
              std::shared_ptr<piles::StockPile> stockPile,
              FoundationPiles foundationPiles,
              TableauPiles tableauPiles);

    void startNewGame();
    const piles::FoundationPile& getFoundationPile(unsigned id) const;
    const piles::TableauPile& getTableauPile(unsigned id) const;
    const piles::StockPile& getStockPile() const;

private:
    cards::Deck::const_iterator
    initializeTableauPilesAndReturnFirstNotUsedCard(const cards::Deck&);

    std::unique_ptr<cards::DeckGenerator> deckGenerator;
    std::shared_ptr<piles::StockPile> stockPile;
    FoundationPiles foundationPiles;
    TableauPiles tableauPiles;
};

}
