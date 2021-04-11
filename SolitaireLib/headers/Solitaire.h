#pragma once

#include <array>
#include <memory>

#include "cards/Card.h"
#include "cards/Cards.h"
#include "cards/Deck.h"
#include "piles/PileId.h"

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
    static constexpr unsigned foundationPilesCount {4};
    static constexpr unsigned tableauPilesCount {7};

    using FoundationPiles =
        std::array<std::shared_ptr<piles::FoundationPile>, foundationPilesCount>;
    using TableauPiles =
        std::array<std::shared_ptr<piles::TableauPile>, tableauPilesCount>;

public:
    Solitaire(std::unique_ptr<cards::DeckGenerator> deckGenerator,
              std::shared_ptr<piles::StockPile> stockPile,
              FoundationPiles foundationPiles,
              TableauPiles tableauPiles);

    void startNewGame();

    void tryPullOutCardFromFoundationPile(const piles::PileId);
    void tryAddCardOnFoundationPile(const piles::PileId);

    void tryUncoverTableauPileTopCard(const piles::PileId);
    void tryPullOutCardsFromTableauPile(const piles::PileId, const unsigned quantity);
    void tryAddCardsOnTableauPile(const piles::PileId);

    void selectNextStockPileCard();
    void tryPullOutCardFromStockPile();

    const piles::FoundationPile& getFoundationPile(const piles::PileId) const;
    const piles::TableauPile& getTableauPile(const piles::PileId) const;
    const piles::StockPile& getStockPile() const;
    const cards::Cards& getCardsInHand() const;

private:
    void initializeFoundationPiles();

    cards::Deck::const_iterator
    initializeTableauPilesAndReturnFirstNotUsedCard(const cards::Deck&);

    void throwExceptionOnInvalidFoundationPileId(const piles::PileId) const;
    void throwExceptionOnInvalidTableauPileId(const piles::PileId) const;

    std::unique_ptr<cards::DeckGenerator> deckGenerator;
    std::shared_ptr<piles::StockPile> stockPile;
    FoundationPiles foundationPiles;
    TableauPiles tableauPiles;
    cards::Cards cardsInHand;
};

}
