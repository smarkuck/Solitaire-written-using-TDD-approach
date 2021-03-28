#include "card/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    TableauPile(card::Cards::const_iterator begin,
                const card::Cards::const_iterator& end);

    void tryAddCards(card::Cards& cardsToAdd);
    card::Cards tryPullOutCards(unsigned quantity);

    const card::Cards& getCards() const;
    unsigned getPlaceInOrderOfFirstCoveredCard() const;

private:
    bool shouldAddCards(const card::Cards& cardsToAdd) const;
    bool isFirstCardKing(const card::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const card::Cards& cardsToAdd) const;

    card::Cards cards;
    unsigned placeInOrderOfFirstCoveredCard;
};

}
}
