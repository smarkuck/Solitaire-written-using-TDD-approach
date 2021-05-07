#pragma once

#include <array>
#include <memory>

#include "Context.h"
#include "geometry/Position.h"
#include "interfaces/Solitaire.h"

namespace solitaire {

class DefaultContext: public Context {
public:
    using FoundationPileColliders = std::array<
        std::unique_ptr<colliders::interfaces::FoundationPileCollider>,
        interfaces::Solitaire::foundationPilesCount>;

    DefaultContext(std::unique_ptr<interfaces::Solitaire>, FoundationPileColliders);

    void setMousePosition(const geometry::Position&) override;
    void setCardsInHandPosition(const geometry::Position&) override;

    interfaces::Solitaire& getSolitaire() override;
    const interfaces::Solitaire& getSolitaire() const override;

    colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) override;

    const colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) const override;

    geometry::Position getMousePosition() const override;
    geometry::Position getCardsInHandPosition() const override;

private:
    std::unique_ptr<interfaces::Solitaire> solitaire;
    FoundationPileColliders foundationPileColliders;
    geometry::Position mousePosition;
    geometry::Position cardsInHandPosition;
};

}
