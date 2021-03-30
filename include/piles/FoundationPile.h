#include <optional>

#include "cards/Cards.h"

namespace solitaire {
namespace piles {

class FoundationPile {
public:
    void initialize();

    void tryAddCard(std::optional<cards::Card>& cardToAdd);
    std::optional<cards::Card> tryPullOutCard();
    void tryRestoreLastPulledOutCard();

    const cards::Cards& getCards() const;
    std::optional<cards::Value> getTopCardValue() const;

private:
    bool shouldAddCard(std::optional<cards::Card>& cardToAdd) const;
    bool isCardToAddAce(const cards::Card& cardToAdd) const;
    bool isCardToAddCorrect(const cards::Card& cardToAdd) const;

    cards::Cards cards;
    std::optional<cards::Card> lastPulledOutCard;
};

}
}
