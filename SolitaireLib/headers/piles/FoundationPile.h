#pragma once

#include <optional>

#include "archivers/Archiver.h"
#include "cards/Cards.h"

namespace solitaire {

namespace cards {
    enum class Value;
}

namespace piles {

class FoundationPile: public archivers::Archiver {
public:
    virtual ~FoundationPile() = default;

    virtual void initialize() = 0;

    virtual void tryAddCard(std::optional<cards::Card>& cardToAdd) = 0;
    virtual std::optional<cards::Card> tryPullOutCard() = 0;

    virtual const cards::Cards& getCards() const = 0;
    virtual std::optional<cards::Value> getTopCardValue() const = 0;
};

}
}
