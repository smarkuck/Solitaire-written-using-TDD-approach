#include <array>

#include "Solitaire.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "graphics/DefaultRenderer.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/Layout.h"
#include "graphics/TextureArea.h"
#include "piles/FoundationPile.h"
#include "piles/PileId.h"
#include "piles/StockPile.h"
#include "piles/TableauPile.h"

using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned cardPlaceholderAlpha {70};

constexpr TextureArea cardPlaceholderTextureArea {TexturePosition {0, 0}, Layout::cardSize};
constexpr TextureArea cardBackTextureArea {TexturePosition {0, 416}, Layout::cardSize};
}

DefaultRenderer::DefaultRenderer(const Solitaire& solitaire,
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

TextureId DefaultRenderer::loadTexture(const std::string& path) const {
    return graphicsSystem->loadTexture(assetsPath + path);
}

void DefaultRenderer::render() const {
    graphicsSystem->renderTextureInFullWindow(backgroundId);

    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        renderFoundationPile(id);

    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id)
        renderTableauPile(id);

    renderStockPile();
    renderCardsInHand();
    graphicsSystem->renderFrame();
}

void DefaultRenderer::renderFoundationPile(const PileId id) const {
    const auto& pileCards = solitaire.getFoundationPile(id).getCards();

    if (pileCards.empty())
        renderCardPlaceholder(getFoundationPilePosition(id));
    else
        renderCard(getFoundationPilePosition(id), pileCards.back());
}

TexturePosition DefaultRenderer::getFoundationPilePosition(const PileId id) const {
    return TexturePosition {
        Layout::firstFoundationPilePositionX + id * Layout::pilesSpacing,
        Layout::foundationPilePositionY
    };
}

void DefaultRenderer::renderTableauPile(const PileId id) const {
    const auto& pile = solitaire.getTableauPile(id);
    const auto& pileCards = pile.getCards();

    if (not pileCards.empty())
        renderTableauPileWithCards(
            getTableauPilePosition(id), pileCards, pile.getTopCoveredCardPosition());
}

void DefaultRenderer::renderTableauPileWithCards(
    TexturePosition pilePosition, const Cards& pileCards,
    const unsigned topCoveredCardPosition) const
{
    auto& cardPosition = pilePosition;

    for (unsigned i = 0; i < topCoveredCardPosition; ++i) {
        renderCardBack(cardPosition);
        cardPosition.y += Layout::coveredTableauPileCardsSpacing;
    }

    for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i) {
        renderCard(cardPosition, pileCards[i]);
        cardPosition.y += Layout::uncoveredTableauPileCardsSpacing;
    }
}

TexturePosition DefaultRenderer::getTableauPilePosition(const PileId id) const {
    return TexturePosition {
        Layout::firstTableauPilePositionX + id * Layout::pilesSpacing,
        Layout::tableauPilePositionY
    };
}

void DefaultRenderer::renderStockPile() const {
    const auto& pile = solitaire.getStockPile();
    const auto& pileCards = pile.getCards();
    const auto& selectedCardIndex = pile.getSelectedCardIndex();

    if (pileCards.empty())
        renderCardPlaceholder(Layout::stockPilePosition);
    else renderStockPileWithCards(pileCards, selectedCardIndex);
}

void DefaultRenderer::renderStockPileWithCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    throwOnInvalidSelectedCardIndex(pileCards, selectedCardIndex);
    renderStockPileCoveredCards(pileCards, selectedCardIndex);
    renderStockPileUncoveredCards(pileCards, selectedCardIndex);
}

void DefaultRenderer::throwOnInvalidSelectedCardIndex(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    if (selectedCardIndex and selectedCardIndex >= pileCards.size())
        throw std::runtime_error {
            "Cannot get Stock pile card with index: " + selectedCardIndex.value()};
}

void DefaultRenderer::renderStockPileCoveredCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    const auto cardsToRender = getStockPileCardsToRenderCount(
        getStockPileCoveredCardsCount(pileCards, selectedCardIndex));

    if (cardsToRender == 0)
        renderCardPlaceholder(Layout::stockPilePosition);

    auto cardPosition = Layout::stockPilePosition;
    for (unsigned i = 0; i < cardsToRender; ++i) {
        renderCardBack(cardPosition);
        cardPosition.x += Layout::stockPileCardsSpacing;
    }
}

void DefaultRenderer::renderStockPileUncoveredCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    const unsigned cardsToRender = getStockPileCardsToRenderCount(
        getStockPileUncoveredCardsCount(selectedCardIndex));

    auto cardPosition = Layout::stockPilePosition;
    cardPosition.x += Layout::pilesSpacing;
    for (unsigned i = 0; i < cardsToRender; ++i) {
        renderCard(cardPosition, pileCards[selectedCardIndex.value()]);
        cardPosition.x += Layout::stockPileCardsSpacing;
    }
}

unsigned DefaultRenderer::getStockPileCoveredCardsCount(const Cards& pileCards,
    const SelectedCardIndex& selectedCardIndex) const
{
    if (selectedCardIndex)
        return pileCards.size() - (selectedCardIndex.value() + 1);
    return pileCards.size();
}

unsigned DefaultRenderer::getStockPileUncoveredCardsCount(
    const SelectedCardIndex& selectedCardIndex) const
{
    return selectedCardIndex ? selectedCardIndex.value() + 1 : 0;
}

unsigned DefaultRenderer::getStockPileCardsToRenderCount(const unsigned cardsCount) const {
    return (cardsCount + 5) / 6;
}

void DefaultRenderer::renderCardsInHand() const {
    auto cardPosition = TexturePosition {0, 0};
    for (const auto& card: solitaire.getCardsInHand()) {
        renderCard(cardPosition, card);
        cardPosition.y += Layout::uncoveredTableauPileCardsSpacing;
    }
}

void DefaultRenderer::renderCard(const TexturePosition& position, const Card& card) const {
    graphicsSystem->renderTexture(
        cardsId, position, getCardTextureArea(card));
}

TextureArea DefaultRenderer::getCardTextureArea(const Card& card) const {
    unsigned x = to_int(card.getValue()) * Layout::cardSize.width;
    unsigned y = to_int(card.getSuit()) * Layout::cardSize.height;
    return TextureArea {TexturePosition{x, y}, Layout::cardSize};
}

void DefaultRenderer::renderCardBack(const TexturePosition& position) const {
    graphicsSystem->renderTexture(cardsId, position, cardBackTextureArea);
}

void DefaultRenderer::renderCardPlaceholder(const TexturePosition& position) const {
    graphicsSystem->renderTexture(
        cardPlaceholderId, position, cardPlaceholderTextureArea);
}

}
