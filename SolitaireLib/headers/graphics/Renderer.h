#pragma once

#include <memory>
#include <string>

#include "graphics/TextureId.h"

namespace solitaire {

class Solitaire;

namespace graphics {

class GraphicsSystem;
struct TextureArea;
struct TexturePosition;

class Renderer {
public:
    Renderer(const Solitaire&, std::unique_ptr<GraphicsSystem>,
             const std::string& assetsPath);

    void render() const;

private:
    TextureId loadTexture(const std::string& path) const;

    void renderFoundationPile(const piles::PileId) const;
    void renderTableauPile(const piles::PileId) const;
    void renderEmptyTableauPile(const piles::PileId) const;
    void renderTableauPileWithCards(TexturePosition pilePosition, const cards::Cards&,
                                    const unsigned topCoveredCardPosition) const;

    TexturePosition getFoundationPilePosition(const piles::PileId) const;
    TexturePosition getTableauPilePosition(const piles::PileId) const;
    TextureArea getCardTextureArea(const cards::Card&) const;

    const Solitaire& solitaire;
    std::unique_ptr<GraphicsSystem> graphicsSystem;
    const std::string assetsPath;

    TextureId backgroundId;
    TextureId cardsId;
    TextureId cardPlaceholderId;
};

}
}
