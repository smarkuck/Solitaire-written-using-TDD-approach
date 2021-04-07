#pragma once

#include "FoundationPile.h"

namespace solitaire::piles {

class DefaultFoundationPile: FoundationPile {
public:
    void initialize() override;

    void tryAddCard(std::optional<cards::Card>& cardToAdd) override;
    std::optional<cards::Card> tryPullOutCard() override;
    void tryRestoreLastPulledOutCard() override;

    const cards::Cards& getCards() const override;
    std::optional<cards::Value> getTopCardValue() const override;

private:
    bool shouldAddCard(std::optional<cards::Card>& cardToAdd) const;
    bool isCardToAddAce(const cards::Card& cardToAdd) const;
    bool isCardToAddCorrect(const cards::Card& cardToAdd) const;

    cards::Cards cards;
    std::optional<cards::Card> lastPulledOutCard;
};

}
