#include "Solitaire.h"

using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire {

Solitaire::Solitaire(std::unique_ptr<CardsGenerator> cardsGenerator,
                     std::unique_ptr<StockPile> stockPile,
                     FoundationPiles foundationPiles,
                     TableauPiles tableauPiles):
    cardsGenerator{std::move(cardsGenerator)},
    stockPile{std::move(stockPile)},
    foundationPiles{std::move(foundationPiles)},
    tableauPiles{std::move(tableauPiles)} {
}

void Solitaire::startNewGame() {
    const auto cards = cardsGenerator->generate();

    for (auto& pile: foundationPiles)
        pile->initialize();

    const auto firstNotUsedCard = initializeTableauPilesAndReturnFirstNotUsedCard(cards);
    stockPile->initialize(firstNotUsedCard, cards.end());
}

Cards::const_iterator
Solitaire::initializeTableauPilesAndReturnFirstNotUsedCard(const Cards& cards) {
    auto firstNotUsedCard = cards.begin();
    unsigned currentPileCardsCount = 1;

    for (auto& pile: tableauPiles) {
        const auto firstPileCard = firstNotUsedCard;
        std::advance(firstNotUsedCard, currentPileCardsCount++);
        pile->initialize(firstPileCard, firstNotUsedCard);
    }

    return firstNotUsedCard;
}

}
