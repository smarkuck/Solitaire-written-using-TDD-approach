#include <array>

#include "Solitaire.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/Renderer.h"
#include "graphics/TextureArea.h"
#include "piles/FoundationPile.h"
#include "piles/PileId.h"
#include "piles/TableauPile.h"

using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned pilesSpacing {89};
constexpr unsigned firstFoundationPilePositionX {283};
constexpr unsigned foundationPilePositionY {30};
constexpr unsigned firstTableauPilePositionX {16};
constexpr unsigned tableauPilePositionY {144};
constexpr unsigned coveredTableauPileCardsSpacing {3};
constexpr unsigned uncoveredTableauPileCardsSpacing {16};
constexpr unsigned cardPlaceholderAlpha {70};

constexpr TextureSize cardSize {75, 104};
constexpr TextureArea cardPlaceholderTextureArea {TexturePosition {0, 0}, cardSize};
constexpr TextureArea cardBackTextureArea {TexturePosition {0, 416}, cardSize};
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
    this->graphicsSystem->setTextureAlpha(cardPlaceholderId, cardPlaceholderAlpha);
}

TextureId Renderer::loadTexture(const std::string& path) const {
    return graphicsSystem->loadTexture(assetsPath + path);
}

void Renderer::render() const {
    graphicsSystem->renderTextureOnFullscreen(backgroundId);

    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        renderFoundationPile(id);

    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id)
        renderTableauPile(id);

    graphicsSystem->renderFrame();
}

void Renderer::renderFoundationPile(const PileId id) const {
    const auto& pileCards = solitaire.getFoundationPile(id).getCards();

    if (pileCards.empty())
        graphicsSystem->renderTexture(
            cardPlaceholderId, getFoundationPilePosition(id), cardPlaceholderTextureArea);
    else
        graphicsSystem->renderTexture(
            cardsId, getFoundationPilePosition(id), getCardTextureArea(pileCards.back()));
}

TexturePosition Renderer::getFoundationPilePosition(const PileId id) const {
    return TexturePosition {
        firstFoundationPilePositionX + id * pilesSpacing,
        foundationPilePositionY
    };
}

void Renderer::renderTableauPile(const PileId id) const {
    const auto& pile = solitaire.getTableauPile(id);
    const auto& pileCards = pile.getCards();

    if (pileCards.empty())
        renderEmptyTableauPile(id);
    else
        renderTableauPileWithCards(
            getTableauPilePosition(id), pileCards, pile.getTopCoveredCardPosition());
}

void Renderer::renderEmptyTableauPile(const PileId id) const {
    graphicsSystem->renderTexture(
        cardPlaceholderId, getTableauPilePosition(id), cardPlaceholderTextureArea);
}

void Renderer::renderTableauPileWithCards(
    TexturePosition pilePosition, const Cards& pileCards,
    const unsigned topCoveredCardPosition) const
{
    auto& cardPosition = pilePosition;

    for (unsigned i = 0; i < topCoveredCardPosition; ++i) {
        graphicsSystem->renderTexture(cardsId, cardPosition, cardBackTextureArea);
        cardPosition.y += coveredTableauPileCardsSpacing;
    }

    for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i) {
        graphicsSystem->renderTexture(
            cardsId, cardPosition, getCardTextureArea(pileCards[i]));
        cardPosition.y += uncoveredTableauPileCardsSpacing;
    }
}

TexturePosition Renderer::getTableauPilePosition(const PileId id) const {
    return TexturePosition {
        firstTableauPilePositionX + id * pilesSpacing,
        tableauPilePositionY
    };
}

TextureArea Renderer::getCardTextureArea(const Card& card) const {
    unsigned x = to_int(card.getValue()) * cardSize.width;
    unsigned y = to_int(card.getSuit()) * cardSize.height;
    return TextureArea {TexturePosition{x, y}, cardSize};
}

}
