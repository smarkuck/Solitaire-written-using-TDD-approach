#pragma once

#include <optional>

#include "cards/Cards.h"
#include "interfaces/archivers/Archiver.h"

namespace solitaire::cards {
    enum class Value;
}

namespace solitaire::piles::interfaces {

class FoundationPile: public archivers::interfaces::Archiver {
public:
    virtual ~FoundationPile() = default;

    virtual void initialize() = 0;

    virtual void tryAddCard(std::optional<cards::Card>& cardToAdd) = 0;
    virtual std::optional<cards::Card> tryPullOutCard() = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual std::optional<cards::Value> getTopCardValue() const = 0;
};

}
