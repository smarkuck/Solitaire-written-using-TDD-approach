#pragma once

#include <memory>
#include <optional>
#include <string>

#include "graphics/Renderer.h"
#include "graphics/TextureId.h"

namespace solitaire {

class Solitaire;

namespace graphics {

class GraphicsSystem;
struct TextureArea;
struct TexturePosition;

class DefaultRenderer: public Renderer {
public:
    DefaultRenderer(const Solitaire&, std::unique_ptr<GraphicsSystem>,
                    const std::string& assetsPath);

    void render() const override;

private:
    using SelectedCardIndex = std::optional<unsigned>;

    TextureId loadTexture(const std::string& path) const;

    void renderFoundationPile(const piles::PileId) const;
    void renderTableauPile(const piles::PileId) const;
    void renderTableauPileWithCards(TexturePosition pilePosition, const cards::Cards&,
                                    const unsigned topCoveredCardPosition) const;
    void renderStockPile() const;
    void renderStockPileWithCards(const cards::Cards&,
                                  const SelectedCardIndex&) const;
    void renderStockPileCoveredCards(const cards::Cards&,
                                     const SelectedCardIndex&) const;
    void renderStockPileUncoveredCards(const cards::Cards&,
                                       const SelectedCardIndex&) const;
    void renderCardsInHand() const;
    void renderCard(const TexturePosition&, const cards::Card&) const;
    void renderCardBack(const TexturePosition&) const;
    void renderCardPlaceholder(const TexturePosition&) const;

    unsigned getStockPileCoveredCardsCount(const cards::Cards&,
        const SelectedCardIndex& selectedCardIndex) const;
    unsigned getStockPileUncoveredCardsCount(
        const SelectedCardIndex& selectedCardIndex) const;
    unsigned getStockPileCardsToRenderCount(const unsigned cardsCount) const;

    TexturePosition getFoundationPilePosition(const piles::PileId) const;
    TexturePosition getTableauPilePosition(const piles::PileId) const;
    TextureArea getCardTextureArea(const cards::Card&) const;

    void throwOnInvalidSelectedCardIndex(const cards::Cards&,
                                         const SelectedCardIndex&) const;

    const Solitaire& solitaire;
    std::unique_ptr<GraphicsSystem> graphicsSystem;
    const std::string assetsPath;

    TextureId backgroundId;
    TextureId cardsId;
    TextureId cardPlaceholderId;
};

}
}
