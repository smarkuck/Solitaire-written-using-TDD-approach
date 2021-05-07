#pragma once

#include <array>
#include <memory>

#include "cards/Cards.h"

namespace solitaire {

namespace piles {
    struct PileId;

namespace interfaces {
    class FoundationPile;
    class StockPile;
    class TableauPile;
}
}

namespace interfaces {

class Solitaire {
public:
    static constexpr unsigned foundationPilesCount {4};
    static constexpr unsigned tableauPilesCount {7};

    using FoundationPiles = std::array<
        std::shared_ptr<piles::interfaces::FoundationPile>, foundationPilesCount>;
    using TableauPiles = std::array<
        std::shared_ptr<piles::interfaces::TableauPile>, tableauPilesCount>;

    virtual ~Solitaire() = default;

    virtual void startNewGame() = 0;

    virtual void tryUndoOperation() = 0;
    virtual void tryPutCardsBackFromHand() = 0;

    virtual void tryPullOutCardFromFoundationPile(const piles::PileId) = 0;
    virtual void tryAddCardOnFoundationPile(const piles::PileId) = 0;

    virtual void tryUncoverTableauPileTopCard(const piles::PileId) = 0;
    virtual void tryPullOutCardsFromTableauPile(
        const piles::PileId, const unsigned quantity) = 0;
    virtual void tryAddCardsOnTableauPile(const piles::PileId) = 0;

    virtual void trySelectNextStockPileCard() = 0;
    virtual void tryPullOutCardFromStockPile() = 0;

    virtual bool isGameFinished() const = 0;

    virtual const piles::interfaces::FoundationPile&
    getFoundationPile(const piles::PileId) const = 0;
    virtual const piles::interfaces::TableauPile&
    getTableauPile(const piles::PileId) const = 0;
    virtual const piles::interfaces::StockPile& getStockPile() const = 0;
    virtual const cards::Cards& getCardsInHand() const = 0;
};

}
}
