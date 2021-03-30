#include "card/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    void initialize(const card::Cards::const_iterator& begin,
                    const card::Cards::const_iterator& end);

    void tryUncoverTopCard();
    void tryAddCards(card::Cards& cardsToAdd);
    card::Cards tryPullOutCards(unsigned quantity);
    void tryRestoreLastPulledOutCards();

    const card::Cards& getCards() const;
    unsigned getPlaceInOrderOfFirstCoveredCard() const;

private:
    bool shouldAddCards(const card::Cards& cardsToAdd) const;
    bool shouldPullOutCards(unsigned quantity) const;
    bool isTopCardCovered() const;
    bool isFirstCardToAddKing(const card::Cards& cardsToAdd) const;
    bool isFirstCardToAddCorrect(const card::Cards& cardsToAdd) const;

    card::Cards cards;
    card::Cards lastPulledOutCards;
    unsigned placeInOrderOfFirstCoveredCard {0};
};

}
}
