#include <optional>

#include "card/Cards.h"

namespace solitaire {
namespace piles {

class FoundationPile {
public:
    void initialize();

    void tryAddCard(std::optional<card::Card>& cardToAdd);
    std::optional<card::Card> tryPullOutCard();
    void tryRestoreLastPulledOutCard();

    const card::Cards& getCards() const;
    std::optional<card::Value> getTopCardValue() const;

private:
    bool shouldAddCard(std::optional<card::Card>& cardToAdd) const;
    bool isCardToAddAce(const card::Card& cardToAdd) const;
    bool isCardToAddCorrect(const card::Card& cardToAdd) const;

    card::Cards cards;
    std::optional<card::Card> lastPulledOutCard;
};

}
}
