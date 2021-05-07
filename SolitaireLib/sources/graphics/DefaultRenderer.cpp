#include <array>

#include "Context.h"
#include "Layout.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "geometry/Area.h"
#include "graphics/DefaultRenderer.h"
#include "interfaces/Solitaire.h"
#include "interfaces/colliders/FoundationPileCollider.h"
#include "interfaces/graphics/GraphicsSystem.h"
#include "interfaces/piles/FoundationPile.h"
#include "interfaces/piles/StockPile.h"
#include "interfaces/piles/TableauPile.h"
#include "piles/PileId.h"

using namespace solitaire::cards;
using namespace solitaire::geometry;
using namespace solitaire::graphics::interfaces;
using namespace solitaire::interfaces;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned cardPlaceholderAlpha {70};

constexpr Area cardPlaceholderTextureArea {Position {0, 0}, Layout::cardSize};
constexpr Area cardBackTextureArea {Position {0, 416}, Layout::cardSize};
}

DefaultRenderer::DefaultRenderer(const Context& context,
                                 std::unique_ptr<GraphicsSystem> graphicsSystem,
                                 const std::string& assetsPath):
    context {context},
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
    const auto& pileCards = context.getSolitaire().getFoundationPile(id).getCards();

    if (pileCards.empty())
        renderCardPlaceholder(getFoundationPilePosition(id));
    else
        renderCard(getFoundationPilePosition(id), pileCards.back());
}

Position DefaultRenderer::getFoundationPilePosition(const PileId id) const {
    return context.getFoundationPileCollider(id).getPosition();
}

void DefaultRenderer::renderTableauPile(const PileId id) const {
    const auto& pile = context.getSolitaire().getTableauPile(id);
    const auto& pileCards = pile.getCards();

    if (not pileCards.empty())
        renderTableauPileWithCards(
            getTableauPilePosition(id), pileCards, pile.getTopCoveredCardPosition());
}

void DefaultRenderer::renderTableauPileWithCards(
    Position pilePosition, const Cards& pileCards,
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

Position DefaultRenderer::getTableauPilePosition(const PileId id) const {
    return Position {
        Layout::firstTableauPilePositionX + static_cast<int>(id) * Layout::pilesSpacing,
        Layout::tableauPilePositionY
    };
}

void DefaultRenderer::renderStockPile() const {
    const auto& pile = context.getSolitaire().getStockPile();
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
    const auto& cards = context.getSolitaire().getCardsInHand();
    if (not cards.empty()) {
        auto cardPosition = context.getCardsInHandPosition();
        for (const auto& card: cards) {
            renderCard(cardPosition, card);
            cardPosition.y += Layout::uncoveredTableauPileCardsSpacing;
        }
    }
}

void DefaultRenderer::renderCard(const Position& position, const Card& card) const {
    graphicsSystem->renderTexture(
        cardsId, position, getCardTextureArea(card));
}

Area DefaultRenderer::getCardTextureArea(const Card& card) const {
    auto x = to_int(card.getValue()) * Layout::cardSize.width;
    auto y = to_int(card.getSuit()) * Layout::cardSize.height;
    return Area {Position {x, y}, Layout::cardSize};
}

void DefaultRenderer::renderCardBack(const Position& position) const {
    graphicsSystem->renderTexture(cardsId, position, cardBackTextureArea);
}

void DefaultRenderer::renderCardPlaceholder(const Position& position) const {
    graphicsSystem->renderTexture(
        cardPlaceholderId, position, cardPlaceholderTextureArea);
}

}
