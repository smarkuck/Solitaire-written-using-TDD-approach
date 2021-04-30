#include <array>

#include "Solitaire.h"
#include "cards/Card.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/Renderer.h"
#include "graphics/TextureArea.h"
#include "graphics/TextureId.h"
#include "piles/FoundationPile.h"
#include "piles/PileId.h"

using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned foundationPilePositionY {30};

constexpr TextureSize cardSize {75, 104};
constexpr TextureArea cardPlaceholderTextureArea {TexturePosition {0, 0}, cardSize};

const std::array<TexturePosition, 4> foundationPilesPositions {
    TexturePosition {283, foundationPilePositionY},
    TexturePosition {372, foundationPilePositionY},
    TexturePosition {461, foundationPilePositionY},
    TexturePosition {550, foundationPilePositionY},
};

}

Renderer::Renderer(const Solitaire& solitaire,
                   std::unique_ptr<GraphicsSystem> graphicsSystem,
                   const std::string& assetsPath):
    solitaire {solitaire},
    graphicsSystem {std::move(graphicsSystem)},
    assetsPath {assetsPath}
{
    this->graphicsSystem->createWindow(windowTitle, windowWidth, windowHeight);
    backgroundId = loadTexture("background.bmp");
    cardsId = loadTexture("cards.bmp");
    cardPlaceholderId = loadTexture("card_placeholder.bmp");
    this->graphicsSystem->setTextureAlpha(cardPlaceholderId, 70);
}

TextureId Renderer::loadTexture(const std::string& path) const {
    return graphicsSystem->loadTexture(assetsPath + path);
}

void Renderer::render() const {
    graphicsSystem->renderTextureOnFullscreen(backgroundId);

    for (PileId id {0}; id < 4; ++id)
        renderFoundationPile(id);

    graphicsSystem->renderFrame();
}

void Renderer::renderFoundationPile(const PileId id) const {
    const auto& pileCards = solitaire.getFoundationPile(id).getCards();

    if (pileCards.empty())
        graphicsSystem->renderTexture(
            cardPlaceholderId, foundationPilesPositions[id], cardPlaceholderTextureArea);
    else
        graphicsSystem->renderTexture(
            cardsId, foundationPilesPositions[id], getCardTextureArea(pileCards.back()));
}

TextureArea Renderer::getCardTextureArea(const Card& card) const {
    unsigned x = static_cast<unsigned>(card.getValue()) * 75;
    unsigned y = static_cast<unsigned>(card.getSuit()) * 104;
    return TextureArea {x, y, 75, 104};
}

}
