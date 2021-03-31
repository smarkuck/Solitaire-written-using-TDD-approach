#include "cards/Cards.h"

namespace solitaire {
namespace piles {

class TableauPile {
public:
    virtual ~TableauPile() = default;

    virtual void initialize(const cards::Cards::const_iterator& begin,
                            const cards::Cards::const_iterator& end) = 0;

    virtual void tryUncoverTopCard() = 0;
    virtual void tryAddCards(cards::Cards& cardsToAdd) = 0;
    virtual cards::Cards tryPullOutCards(unsigned quantity) = 0;
    virtual void tryRestoreLastPulledOutCards() = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual unsigned getPlaceInOrderOfFirstCoveredCard() const = 0;
};

}
}
