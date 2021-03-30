#include <optional>

#include "card/Cards.h"

namespace solitaire {
namespace piles {

class StockPile {
public:
    void initialize(const card::Cards::const_iterator& begin,
                    const card::Cards::const_iterator& end);

    void selectNextCard();
    std::optional<card::Card> tryPullOutCard();
    void tryRestoreLastPulledOutCard();

    const card::Cards& getCards() const;
    std::optional<unsigned> getSelectedCardIndex() const;

private:
    void incrementSelectedCardIndex();
    void decrementSelectedCardIndex();
    void restoreLastPulledOutCard();

    card::Cards cards;
    std::optional<card::Card> lastPulledOutCard;
    std::optional<unsigned> selectedCardIndex;
};

}
}
