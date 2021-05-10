#include <array>

#include "Layout.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "geometry/Area.h"
#include "graphics/Renderer.h"
#include "interfaces/Button.h"
#include "interfaces/Context.h"
#include "interfaces/Solitaire.h"
#include "interfaces/colliders/FoundationPileCollider.h"
#include "interfaces/colliders/StockPileCollider.h"
#include "interfaces/colliders/TableauPileCollider.h"
#include "interfaces/graphics/GraphicsSystem.h"
#include "interfaces/piles/FoundationPile.h"
#include "interfaces/piles/StockPile.h"
#include "interfaces/piles/TableauPile.h"
#include "piles/PileId.h"

using namespace solitaire::cards;
using namespace solitaire::colliders::interfaces;
using namespace solitaire::geometry;
using namespace solitaire::graphics::interfaces;
using namespace solitaire::interfaces;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned noAlpha {255};
constexpr unsigned cardPlaceholderAlpha {70};
constexpr unsigned buttonAlpha {150};

constexpr Area cardPlaceholderTextureArea {Position {0, 0}, Layout::cardSize};
constexpr Area cardBackTextureArea {Position {0, 416}, Layout::cardSize};
}

Renderer::Renderer(const Context& context,
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
    winId = loadTexture("win.bmp");
    newGameId = loadTexture("new_game.bmp");
    this->graphicsSystem->setTextureAlpha(newGameId, buttonAlpha);
    undoId = loadTexture("undo.bmp");
    this->graphicsSystem->setTextureAlpha(undoId, buttonAlpha);
}

TextureId Renderer::loadTexture(const std::string& path) const {
    return graphicsSystem->loadTexture(assetsPath + path);
}

void Renderer::render() const {
    graphicsSystem->renderTextureInFullWindow(backgroundId);
    renderButtons();

    if (context.getSolitaire().isGameFinished())
        graphicsSystem->renderTextureInFullWindow(winId);
    else
        renderPiles();

    graphicsSystem->renderFrame();
}

void Renderer::renderButtons() const {
    setButtonsAlpha();

    graphicsSystem->renderTexture(
        newGameId, Layout::newGameButtonPosition,
        Area {Position {0, 0}, Layout::newGameButtonSize});

    graphicsSystem->renderTexture(
        undoId, Layout::undoButtonPosition,
        Area {Position {0, 0}, Layout::undoButtonSize});
}

void Renderer::setButtonsAlpha() const {
    if (context.getNewGameButton().isHovered())
        graphicsSystem->setTextureAlpha(newGameId, noAlpha);
    else
        graphicsSystem->setTextureAlpha(newGameId, buttonAlpha);
}

void Renderer::renderPiles() const {
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        renderFoundationPile(id);

    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id)
        renderTableauPile(id);

    renderStockPile();
    renderCardsInHand();
}

void Renderer::renderFoundationPile(const PileId id) const {
    const auto& pileCards = context.getSolitaire().getFoundationPile(id).getCards();

    if (pileCards.empty())
        renderCardPlaceholder(getFoundationPilePosition(id));
    else
        renderCard(getFoundationPilePosition(id), pileCards.back());
}

Position Renderer::getFoundationPilePosition(const PileId id) const {
    return context.getFoundationPileCollider(id).getPosition();
}

void Renderer::renderTableauPile(const PileId id) const {
    const auto& pile = context.getSolitaire().getTableauPile(id);
    const auto& pileCards = pile.getCards();
    const auto& pileCollider = context.getTableauPileCollider(id);

    if (not pileCards.empty())
        renderTableauPileWithCards(
            pileCollider,pileCards, pile.getTopCoveredCardPosition());
}

void Renderer::renderTableauPileWithCards(
    const TableauPileCollider& pileCollider, const Cards& pileCards,
    const unsigned topCoveredCardPosition) const
{
    for (unsigned i = 0; i < topCoveredCardPosition; ++i)
        renderCardBack(pileCollider.getCardPosition(i));

    for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i)
        renderCard(pileCollider.getCardPosition(i), pileCards[i]);
}

void Renderer::renderStockPile() const {
    const auto& pile = context.getSolitaire().getStockPile();
    const auto& pileCards = pile.getCards();
    const auto& selectedCardIndex = pile.getSelectedCardIndex();

    if (pileCards.empty())
        renderCardPlaceholder(Layout::stockPilePosition);
    else renderStockPileWithCards(pileCards, selectedCardIndex);
}

void Renderer::renderStockPileWithCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    throwOnInvalidSelectedCardIndex(pileCards, selectedCardIndex);
    const auto& collider = context.getStockPileCollider();
    renderStockPileCoveredCardsOrPlaceholder(pileCards, selectedCardIndex, collider);
    renderStockPileUncoveredCards(pileCards, selectedCardIndex, collider);
}

void Renderer::throwOnInvalidSelectedCardIndex(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    if (selectedCardIndex and selectedCardIndex >= pileCards.size())
        throw std::runtime_error {
            "Cannot get Stock pile card with index: " + selectedCardIndex.value()};
}

void Renderer::renderStockPileCoveredCardsOrPlaceholder(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex,
    const StockPileCollider& collider) const
{
    if (areCoveredCardsEmpty(pileCards, selectedCardIndex)) {
        renderCardPlaceholder(Layout::stockPilePosition);
        return;
    }

    renderStockPileCoveredCards(pileCards, selectedCardIndex, collider);
}

bool Renderer::areCoveredCardsEmpty(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex) const
{
    return selectedCardIndex and selectedCardIndex == pileCards.size() - 1;
}

void Renderer::renderStockPileCoveredCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex,
    const StockPileCollider& collider) const
{
    auto cardPosition = Layout::stockPilePosition;
    const auto coveredCardsPosition = collider.getCoveredCardsPosition();

    while (cardPosition.x <= coveredCardsPosition.x) {
        renderCardBack(cardPosition);
        cardPosition.x += Layout::stockPileCardsSpacing;
    }
}

void Renderer::renderStockPileUncoveredCards(
    const Cards& pileCards, const SelectedCardIndex& selectedCardIndex,
    const StockPileCollider& collider) const
{
    if (not selectedCardIndex) return;

    auto cardPosition = Layout::stockPilePosition;
    cardPosition.x += Layout::pilesSpacing;
    const auto uncoveredCardsPosition = collider.getUncoveredCardsPosition();

    while (cardPosition.x <= uncoveredCardsPosition.x) {
        renderCard(cardPosition, pileCards[selectedCardIndex.value()]);
        cardPosition.x += Layout::stockPileCardsSpacing;
    }
}

void Renderer::renderCardsInHand() const {
    const auto& cards = context.getSolitaire().getCardsInHand();
    if (not cards.empty()) {
        auto cardPosition = context.getCardsInHandPosition();
        for (const auto& card: cards) {
            renderCard(cardPosition, card);
            cardPosition.y += Layout::uncoveredTableauPileCardsSpacing;
        }
    }
}

void Renderer::renderCard(const Position& position, const Card& card) const {
    graphicsSystem->renderTexture(
        cardsId, position, getCardTextureArea(card));
}

Area Renderer::getCardTextureArea(const Card& card) const {
    auto x = to_int(card.getValue()) * Layout::cardSize.width;
    auto y = to_int(card.getSuit()) * Layout::cardSize.height;
    return Area {Position {x, y}, Layout::cardSize};
}

void Renderer::renderCardBack(const Position& position) const {
    graphicsSystem->renderTexture(cardsId, position, cardBackTextureArea);
}

void Renderer::renderCardPlaceholder(const Position& position) const {
    graphicsSystem->renderTexture(
        cardPlaceholderId, position, cardPlaceholderTextureArea);
}

}
