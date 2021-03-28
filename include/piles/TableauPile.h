#include "card/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    TableauPile(card::Cards::const_iterator begin, const card::Cards::const_iterator& end);

    void tryAddCards(card::Cards& cardsToAdd);

    const card::Cards& getCards() const;
    unsigned getQueuePositionOfFirstFaceDownCard() const;

private:
    bool shouldAddCards(const card::Cards& cardsToAdd) const;
    bool isFirstCardKing(const card::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const card::Cards& cardsToAdd) const;

    card::Cards cards;
    unsigned queuePositionOfFirstFaceDownCard;
};

}
}
