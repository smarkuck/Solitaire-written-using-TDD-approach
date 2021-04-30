#pragma once

#include "Solitaire.h"
#include "gmock/gmock.h"
#include "piles/PileId.h"

namespace solitaire {

class SolitaireMock: public Solitaire {
public:
    MOCK_METHOD(void, startNewGame, (), (override));

    MOCK_METHOD(void, tryUndoOperation, (), (override));
    MOCK_METHOD(void, tryPutCardsBackFromHand, (), (override));

    MOCK_METHOD(void, tryPullOutCardFromFoundationPile, (const piles::PileId), (override));
    MOCK_METHOD(void, tryAddCardOnFoundationPile, (const piles::PileId), (override));

    MOCK_METHOD(void, tryUncoverTableauPileTopCard, (const piles::PileId), (override));
    MOCK_METHOD(void, tryPullOutCardsFromTableauPile,
                (const piles::PileId, const unsigned), (override));
    MOCK_METHOD(void, tryAddCardsOnTableauPile, (const piles::PileId), (override));

    MOCK_METHOD(void, trySelectNextStockPileCard, (), (override));
    MOCK_METHOD(void, tryPullOutCardFromStockPile, (), (override));

    MOCK_METHOD(bool, isGameFinished, (), (const, override));

    MOCK_METHOD(const piles::FoundationPile&, getFoundationPile,
                (const piles::PileId), (const, override));
    MOCK_METHOD(const piles::TableauPile&, getTableauPile,
                (const piles::PileId), (const, override));
    MOCK_METHOD(const piles::StockPile&, getStockPile, (), (const, override));
    MOCK_METHOD(const cards::Cards&, getCardsInHand, (), (const, override));
};

}
