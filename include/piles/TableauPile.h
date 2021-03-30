#include "cards/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    void initialize(const cards::Cards::const_iterator& begin,
                    const cards::Cards::const_iterator& end);

    void tryUncoverTopCard();
    void tryAddCards(cards::Cards& cardsToAdd);
    cards::Cards tryPullOutCards(unsigned quantity);
    void tryRestoreLastPulledOutCards();

    const cards::Cards& getCards() const;
    unsigned getPlaceInOrderOfFirstCoveredCard() const;

private:
    bool shouldAddCards(const cards::Cards& cardsToAdd) const;
    bool shouldPullOutCards(unsigned quantity) const;
    bool isTopCardCovered() const;
    bool isFirstCardToAddKing(const cards::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const cards::Cards& cardsToAdd) const;

    cards::Cards cards;
    cards::Cards lastPulledOutCards;
    unsigned placeInOrderOfFirstCoveredCard {0};
};

}
}
