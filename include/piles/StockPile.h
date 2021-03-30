#include <optional>

#include "cards/Cards.h"

namespace solitaire {
namespace piles {

class StockPile {
public:
    void initialize(const cards::Cards::const_iterator& begin,
                    const cards::Cards::const_iterator& end);

    void selectNextCard();
    std::optional<cards::Card> tryPullOutCard();
    void tryRestoreLastPulledOutCard();

    const cards::Cards& getCards() const;
    std::optional<unsigned> getSelectedCardIndex() const;

private:
    void incrementSelectedCardIndex();
    void decrementSelectedCardIndex();
    void restoreLastPulledOutCard();

    cards::Cards cards;
    std::optional<cards::Card> lastPulledOutCard;
    std::optional<unsigned> selectedCardIndex;
};

}
}
