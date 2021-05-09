#pragma once

#include <memory>
#include <optional>
#include <string>

#include "cards/Cards.h"
#include "graphics/TextureId.h"
#include "interfaces/graphics/Renderer.h"

namespace solitaire::colliders::interfaces {
class StockPileCollider;
class TableauPileCollider;
}

namespace solitaire::geometry {
struct Area;
struct Position;
}

namespace solitaire::graphics::interfaces {
class GraphicsSystem;
}

namespace solitaire::interfaces {
class Context;
}

namespace solitaire::graphics {

class Renderer: public interfaces::Renderer {
public:
    Renderer(const solitaire::interfaces::Context&,
             std::unique_ptr<interfaces::GraphicsSystem>,
             const std::string& assetsPath);

    void render() const override;

private:
    using SelectedCardIndex = std::optional<unsigned>;

    TextureId loadTexture(const std::string& path) const;

    void renderButtons() const;
    void renderPiles() const;
    void renderFoundationPile(const piles::PileId) const;
    void renderTableauPile(const piles::PileId) const;
    void renderTableauPileWithCards(
        const colliders::interfaces::TableauPileCollider&, const cards::Cards&,
        const unsigned topCoveredCardPosition) const;
    void renderStockPile() const;
    void renderStockPileWithCards(
        const cards::Cards&, const SelectedCardIndex&) const;
    void renderStockPileCoveredCardsOrPlaceholder(
        const cards::Cards&, const SelectedCardIndex&,
        const colliders::interfaces::StockPileCollider&) const;
    void renderStockPileCoveredCards(
        const cards::Cards&, const SelectedCardIndex&,
        const colliders::interfaces::StockPileCollider&) const;
    void renderStockPileUncoveredCards(
        const cards::Cards&, const SelectedCardIndex&,
        const colliders::interfaces::StockPileCollider&) const;
    void renderCardsInHand() const;
    void renderCard(const geometry::Position&, const cards::Card&) const;
    void renderCardBack(const geometry::Position&) const;
    void renderCardPlaceholder(const geometry::Position&) const;

    geometry::Position getFoundationPilePosition(const piles::PileId) const;
    geometry::Position getTableauPilePosition(const piles::PileId) const;
    geometry::Area getCardTextureArea(const cards::Card&) const;

    bool areCoveredCardsEmpty(const cards::Cards&, const SelectedCardIndex&) const;

    void throwOnInvalidSelectedCardIndex(const cards::Cards&,
                                         const SelectedCardIndex&) const;

    const solitaire::interfaces::Context& context;
    std::unique_ptr<interfaces::GraphicsSystem> graphicsSystem;
    const std::string assetsPath;

    TextureId backgroundId;
    TextureId cardsId;
    TextureId cardPlaceholderId;
    TextureId winId;
    TextureId newGameId;
    TextureId undoId;
};

}
