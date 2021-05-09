#pragma once

#include <array>
#include <memory>

#include "geometry/Position.h"
#include "interfaces/Context.h"
#include "interfaces/Solitaire.h"

namespace solitaire {

class Context: public interfaces::Context {
public:
    using FoundationPileColliders = std::array<
        std::unique_ptr<colliders::interfaces::FoundationPileCollider>,
        interfaces::Solitaire::foundationPilesCount>;

    using TableauPileColliders = std::array<
        std::unique_ptr<colliders::interfaces::TableauPileCollider>,
        interfaces::Solitaire::tableauPilesCount>;

    Context(std::unique_ptr<interfaces::Solitaire>,
            FoundationPileColliders, TableauPileColliders,
            std::unique_ptr<colliders::interfaces::StockPileCollider>);

    void setMousePosition(const geometry::Position&) override;
    void setCardsInHandPosition(const geometry::Position&) override;

    interfaces::Solitaire& getSolitaire() override;
    const interfaces::Solitaire& getSolitaire() const override;

    colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) override;

    const colliders::interfaces::FoundationPileCollider&
    getFoundationPileCollider(const piles::PileId) const override;

    colliders::interfaces::TableauPileCollider&
    getTableauPileCollider(const piles::PileId) override;

    const colliders::interfaces::TableauPileCollider&
    getTableauPileCollider(const piles::PileId) const override;

    colliders::interfaces::StockPileCollider&
    getStockPileCollider() override;;

    const colliders::interfaces::StockPileCollider&
    getStockPileCollider() const override;

    geometry::Position getMousePosition() const override;
    geometry::Position getCardsInHandPosition() const override;

private:
    std::unique_ptr<interfaces::Solitaire> solitaire;
    FoundationPileColliders foundationPileColliders;
    TableauPileColliders tableauPileColliders;
    std::unique_ptr<colliders::interfaces::StockPileCollider> stockPileCollider;
    geometry::Position mousePosition;
    geometry::Position cardsInHandPosition;
};

}
