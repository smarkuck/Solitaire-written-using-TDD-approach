#include "card/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    TableauPile(card::Cards::const_iterator begin, const card::Cards::const_iterator& end) {
        cards.assign(begin, end);
        queuePositionOfFirstFaceDownCard = cards.size() > 0 ? cards.size() - 1 : 0;
    }

    void tryAddCards(card::Cards& cardsToAdd) {
        if (shouldAddCards(cardsToAdd)) {
            cards.insert(cards.end(), cardsToAdd.begin(), cardsToAdd.end());
            cardsToAdd.clear();
        }
    }

    const card::Cards& getCards() const {
        return cards;
    }

    unsigned getQueuePositionOfFirstFaceDownCard() const {
        return queuePositionOfFirstFaceDownCard;
    }

private:
    bool shouldAddCards(const card::Cards& cardsToAdd) const {
        if (cardsToAdd.empty()) return false;
        if (cards.empty()) return isFirstCardKing(cardsToAdd);
        return isFirstCardToAddCorrect(cardsToAdd);
    }

    bool isFirstCardKing(const card::Cards& cardsToAdd) const {
        return cardsToAdd.front().getValue() == card::Value::King;
    }

    bool isFirstCardToAddCorrect(const card::Cards& cardsToAdd) const {
        const card::Card& topPileCard = cards.back();
        const card::Card& firstCardToAdd = cardsToAdd.front();

        return topPileCard.hasValueOneGreaterThan(firstCardToAdd) and
               topPileCard.hasDifferentColorThan(firstCardToAdd);
    }

    card::Cards cards;
    unsigned queuePositionOfFirstFaceDownCard;
};

}
}
