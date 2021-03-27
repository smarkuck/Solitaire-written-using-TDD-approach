#include "card/cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    TableauPile(card::Cards::const_iterator begin, const card::Cards::const_iterator& end) {
        cards.assign(begin, end);
        queuePositionOfFirstFaceDownCard = cards.size() > 0 ? cards.size() - 1 : 0;
    }

    void tryAddCards(card::Cards& cardsToAdd) {
        if (cardsToAdd.empty()) return;

        if (cardsToAdd.at(0).value == card::Value::King) {
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
    card::Cards cards;
    unsigned queuePositionOfFirstFaceDownCard;
};

}
}
