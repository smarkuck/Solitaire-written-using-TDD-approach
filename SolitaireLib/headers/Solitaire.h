#pragma once

#include "cards/Cards.h"

namespace solitaire {

namespace piles {
    class FoundationPile;
    struct PileId;
    class StockPile;
    class TableauPile;
}

class Solitaire {
public:
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

    virtual const piles::FoundationPile& getFoundationPile(const piles::PileId) const = 0;
    virtual const piles::TableauPile& getTableauPile(const piles::PileId) const = 0;
    virtual const piles::StockPile& getStockPile() const = 0;
    virtual const cards::Cards& getCardsInHand() const = 0;
};

}
