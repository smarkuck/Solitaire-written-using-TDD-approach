#include "StockPile.h"

namespace solitaire {
namespace piles {

class DefaultStockPile: public StockPile {
public:
    void initialize(const cards::Cards::const_iterator& begin,
                    const cards::Cards::const_iterator& end) override;

    void selectNextCard() override;
    std::optional<cards::Card> tryPullOutCard() override;
    void tryRestoreLastPulledOutCard() override;

    const cards::Cards& getCards() const override;
    std::optional<unsigned> getSelectedCardIndex() const override;

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
