#pragma once

#include <memory>

#include "interfaces/archivers/Snapshot.h"
#include "interfaces/piles/FoundationPile.h"

namespace solitaire::piles {

class DefaultFoundationPile: public std::enable_shared_from_this<DefaultFoundationPile>,
                             public interfaces::FoundationPile {
public:
    void initialize() override;
    std::unique_ptr<archivers::interfaces::Snapshot> createSnapshot() override;

    void tryAddCard(std::optional<cards::Card>& cardToAdd) override;
    std::optional<cards::Card> tryPullOutCard() override;

    const cards::Cards& getCards() const override;
    std::optional<cards::Value> getTopCardValue() const override;

private:
    class Snapshot;

    bool shouldAddCard(std::optional<cards::Card>& cardToAdd) const;
    bool isCardToAddAce(const cards::Card& cardToAdd) const;
    bool isCardToAddCorrect(const cards::Card& cardToAdd) const;

    cards::Cards cards;
};

class DefaultFoundationPile::Snapshot: public archivers::interfaces::Snapshot {
public:
    Snapshot(std::shared_ptr<DefaultFoundationPile> foundationPile,
             cards::Cards pileCards);

    void restore() const override;
    bool isSnapshotOfSameObject(
        const archivers::interfaces::Snapshot&) const override;

private:
    const std::shared_ptr<DefaultFoundationPile> foundationPile;
    const cards::Cards pileCards;
};

}
