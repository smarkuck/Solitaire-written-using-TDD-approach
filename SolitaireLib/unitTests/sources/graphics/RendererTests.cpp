#include <array>

#include "ButtonMock.h"
#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "colliders/FoundationPileColliderMock.h"
#include "colliders/StockPileColliderMock.h"
#include "colliders/TableauPileColliderMock.h"
#include "gmock/gmock.h"
#include "geometry/Area.h"
#include "graphics/GraphicsSystemMock.h"
#include "graphics/Renderer.h"
#include "interfaces/archivers/Snapshot.h"
#include "piles/FoundationPileMock.h"
#include "piles/PileId.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::colliders;
using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string assetsPath {"dummyPath/"};
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned noAlpha {255};
constexpr unsigned cardPlaceholderAlpha {70};
constexpr unsigned buttonAlpha {150};
constexpr unsigned foundationPilesCount {4};
constexpr unsigned tableauPilesCount {7};
constexpr unsigned cardsInHandSpacing {16};
constexpr unsigned stockPileCardsSpacing {2};

const TextureId backgroundTextureId {0};
const TextureId cardsTextureId {1};
const TextureId cardPlaceholderTextureId {2};
const TextureId winId {3};
const TextureId newGameId {4};
const TextureId undoId {5};

constexpr Size cardSize {75, 104};

constexpr Area newGameTextureArea {Position {0, 0}, Size {58, 17}};
constexpr Area undoTextureArea {Position {0, 0}, Size {34, 17}};
constexpr Area cardPlaceholderTextureArea {Position {0, 0}, cardSize};
constexpr Area cardBackTextureArea {Position {0, 416}, cardSize};

constexpr Position newGameButtonPosition {16, 7};
constexpr Position undoButtonPosition {80, 7};
constexpr Position pilePosition {283, 30};
constexpr Position stockPilePosition {16, 30};
constexpr Position stockPileLastCoveredCardPosition {20, 30};
constexpr Position stockPileUncoveredCardsPosition {105, 30};
constexpr Position stockPileLastUncoveredCardPosition {109, 30};
constexpr Position cardsInHandPosition {15, 22};

const Card fiveDiamond {Value::Five, Suit::Diamond};

const Cards noCards;

const Cards threeCards {
    Card {Value::Ace, Suit::Heart},
    Card {Value::Seven, Suit::Spade},
    Card {Value::King, Suit::Club}
};
}

class RendererTests: public Test {
public:
    RendererTests() {
        EXPECT_CALL(*graphicsSystemMock,
            createWindow(windowTitle, windowWidth, windowHeight));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "background.bmp"))
            .WillOnce(Return(backgroundTextureId));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "cards.bmp"))
            .WillOnce(Return(cardsTextureId));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "card_placeholder.bmp"))
            .WillOnce(Return(cardPlaceholderTextureId));
        EXPECT_CALL(*graphicsSystemMock,
            setTextureAlpha(cardPlaceholderTextureId, cardPlaceholderAlpha));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "win.bmp"))
            .WillOnce(Return(winId));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "new_game.bmp"))
            .WillOnce(Return(newGameId));
        EXPECT_CALL(*graphicsSystemMock,
            setTextureAlpha(newGameId, buttonAlpha));
        EXPECT_CALL(*graphicsSystemMock, loadTexture(assetsPath + "undo.bmp"))
            .WillOnce(Return(undoId));
        EXPECT_CALL(*graphicsSystemMock,
            setTextureAlpha(undoId, buttonAlpha));
    }

    void expectRenderBackgroundAndUnhoveredButtons() {
        EXPECT_CALL(*graphicsSystemMock, renderTextureInFullWindow(backgroundTextureId));
        EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
        EXPECT_CALL(buttonMock, isHovered()).WillOnce(Return(false));
        EXPECT_CALL(*graphicsSystemMock, setTextureAlpha(newGameId, buttonAlpha));
        expectRenderButtons();
    }

    void expectRenderButtons() {
        EXPECT_CALL(*graphicsSystemMock,
            renderTexture(newGameId, newGameButtonPosition, newGameTextureArea));
        EXPECT_CALL(*graphicsSystemMock,
            renderTexture(undoId, undoButtonPosition, undoTextureArea));
    }

    InSequence seq;
    const ContextMock contextMock;
    SolitaireMock solitaireMock;
    ButtonMock buttonMock;
    mock_ptr<GraphicsSystemMock> graphicsSystemMock;
};

TEST_F(RendererTests, onConstructionShouldCreateWindowAndLoadAllTextures) {
    Renderer {contextMock, graphicsSystemMock.make_unique(), assetsPath};
}

TEST_F(RendererTests, ifGameIsFinishedRenderWinTexture) {
    expectRenderBackgroundAndUnhoveredButtons();
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, isGameFinished()).WillOnce(Return(true));
    EXPECT_CALL(*graphicsSystemMock, renderTextureInFullWindow(winId));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    Renderer {contextMock, graphicsSystemMock.make_unique(), assetsPath}.render();
}

TEST_F(RendererTests, renderHoveredNewGameButton) {
    EXPECT_CALL(*graphicsSystemMock,
        renderTextureInFullWindow(backgroundTextureId));
    EXPECT_CALL(contextMock, getNewGameButton()).WillOnce(ReturnRef(buttonMock));
    EXPECT_CALL(buttonMock, isHovered()).WillOnce(Return(true));
    EXPECT_CALL(*graphicsSystemMock, setTextureAlpha(newGameId, noAlpha));
    expectRenderButtons();
    EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
    EXPECT_CALL(solitaireMock, isGameFinished()).WillOnce(Return(true));
    EXPECT_CALL(*graphicsSystemMock, renderTextureInFullWindow(winId));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    Renderer {contextMock, graphicsSystemMock.make_unique(), assetsPath}.render();
}

class CreatedRendererTests: public RendererTests {
public:
    using TopCoveredCardPosition = unsigned;

    CreatedRendererTests() {
        expectRenderBackgroundAndUnhoveredButtons();
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, isGameFinished()).WillOnce(Return(false));

        expectRenderFoundationPile(PileId {0}, noCards);
        expectRenderFoundationPile(PileId {1}, threeCards);
        expectRenderFoundationPile(PileId {2}, threeCards);
        expectRenderFoundationPile(PileId {3}, threeCards);

        expectRenderTableauPile(PileId {0}, noCards);
        expectRenderTableauPile(PileId {1}, threeCards, TopCoveredCardPosition {0});
        expectRenderTableauPile(PileId {2}, threeCards, TopCoveredCardPosition {1});
        expectRenderTableauPile(PileId {3}, threeCards, TopCoveredCardPosition {2});
        expectRenderTableauPile(PileId {4}, threeCards, TopCoveredCardPosition {3});
        expectRenderTableauPile(PileId {5}, threeCards, TopCoveredCardPosition {4});
        expectRenderTableauPile(PileId {6}, threeCards, TopCoveredCardPosition {5});
    }

    void expectRenderFoundationPile(const PileId id, const Cards& pileCards) {
        EXPECT_CALL(contextMock, getSolitaire())
            .WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getFoundationPile(id))
            .WillOnce(ReturnRef(foundationPileMocks[id]));
        EXPECT_CALL(foundationPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));
        EXPECT_CALL(contextMock, getFoundationPileCollider(id))
            .WillOnce(ReturnRef(foundationPileColliderMock));
        EXPECT_CALL(foundationPileColliderMock, getPosition()).WillOnce(Return(pilePosition));

        if (pileCards.empty())
            expectRenderCardPlaceholder(pilePosition);
        else
            expectRenderCard(pilePosition, pileCards.back());
    }

    void expectRenderTableauPile(const PileId id, const Cards& pileCards,
                                 const TopCoveredCardPosition topCoveredCardPosition = 0)
    {
        EXPECT_CALL(contextMock, getSolitaire())
            .WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getTableauPile(id))
            .WillOnce(ReturnRef(tableauPileMocks[id]));
        EXPECT_CALL(tableauPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));
        EXPECT_CALL(contextMock, getTableauPileCollider(id))
            .WillOnce(ReturnRef(tableauPileColliderMock));

        if (not pileCards.empty())
            expectRenderTableauPileWithCards(id, pileCards, topCoveredCardPosition);
    }

    void expectRenderTableauPileWithCards(const PileId id, const Cards& pileCards,
                                          const TopCoveredCardPosition topCoveredCardPosition)
    {
        EXPECT_CALL(tableauPileMocks[id], getTopCoveredCardPosition())
            .WillOnce(Return(topCoveredCardPosition));

        for (unsigned i = 0; i < topCoveredCardPosition; ++i) {
            EXPECT_CALL(tableauPileColliderMock, getCardPosition(i))
                .WillOnce(Return(pilePosition));
            expectRenderCardBack(pilePosition);
        }

        for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i) {
            EXPECT_CALL(tableauPileColliderMock, getCardPosition(i))
                .WillOnce(Return(pilePosition));
            expectRenderCard(pilePosition, pileCards[i]);
        }
    }

    void expectGetStockPileData(const Cards& pileCards,
                                const std::optional<unsigned>& selectedCardIndex)
    {
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getStockPile()).WillOnce(ReturnRef(stockPileMock));
        EXPECT_CALL(stockPileMock, getCards()).WillOnce(ReturnRef(pileCards));
        EXPECT_CALL(stockPileMock, getSelectedCardIndex()).WillOnce(Return(selectedCardIndex));
    }

    void expectGetStockPileCollider() {
        EXPECT_CALL(contextMock, getStockPileCollider())
            .WillOnce(ReturnRef(stockPileColliderMock));
    }

    void expectRenderStockPileCoveredCards() {
        EXPECT_CALL(stockPileColliderMock, getCoveredCardsPosition())
            .WillOnce(Return(stockPileLastCoveredCardPosition));

        auto cardPosition = stockPilePosition;
        while (cardPosition.x <= stockPileLastCoveredCardPosition.x) {
            expectRenderCardBack(cardPosition);
            cardPosition.x += stockPileCardsSpacing;
        }
    }

    void expectRenderStockPileUncoveredCards() {
        EXPECT_CALL(stockPileColliderMock, getUncoveredCardsPosition)
            .WillOnce(Return(stockPileLastUncoveredCardPosition));

        auto cardPosition = stockPileUncoveredCardsPosition;
        while (cardPosition.x <= stockPileLastUncoveredCardPosition.x) {
            expectRenderCard(cardPosition, fiveDiamond);
            cardPosition.x += stockPileCardsSpacing;
        }
    }

    void expectGetCardsInHand(const Cards& cardsInHand) {
        EXPECT_CALL(contextMock, getSolitaire()).WillOnce(ReturnRef(solitaireMock));
        EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(cardsInHand));
    }

    void expectRenderCard(const Position& position, const Card& card) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardsTextureId, position, getCardTextureArea(card)));
    }

    void expectRenderCardBack(const Position& position) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardsTextureId, position, cardBackTextureArea));
    }

    void expectRenderCardPlaceholder(const Position& position) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardPlaceholderTextureId, position, cardPlaceholderTextureArea));
    }

    Area getCardTextureArea(const Card& card) {
        auto x = to_int(card.getValue()) * cardSize.width;
        auto y = to_int(card.getSuit()) * cardSize.height;
        return Area {Position {x, y}, cardSize};
    }

    Cards getPileCardsWithSelectedFiveDiamond(
        unsigned quantity, unsigned selectedCardIndex)
    {
        Cards cards {quantity - 1, Card {Value::Ace, Suit::Heart}};
        cards.insert(std::next(cards.begin(), selectedCardIndex), fiveDiamond);
        return cards;
    }

    Renderer renderer {contextMock, graphicsSystemMock.make_unique(), assetsPath};
    std::array<FoundationPileMock, foundationPilesCount> foundationPileMocks;
    FoundationPileColliderMock foundationPileColliderMock;
    std::array<TableauPileMock, tableauPilesCount> tableauPileMocks;
    TableauPileColliderMock tableauPileColliderMock;
    StockPileColliderMock stockPileColliderMock;
    StockPileMock stockPileMock;
};

TEST_F(CreatedRendererTests, renderSolitaireWithEmptyStockPile) {
    expectGetStockPileData(noCards, std::nullopt);
    expectRenderCardPlaceholder(stockPilePosition);
    expectGetCardsInHand(noCards);
    EXPECT_CALL(*graphicsSystemMock, renderFrame());
    renderer.render();
}

TEST_F(CreatedRendererTests, throwOnInvalidStockPileSelectedCardIndex) {
    expectGetStockPileData(threeCards, 3);
    EXPECT_THROW(renderer.render(), std::runtime_error);
}

TEST_F(CreatedRendererTests, renderStockPileWithCoveredCards) {
    expectGetStockPileData(threeCards, std::nullopt);
    expectGetStockPileCollider();
    expectRenderStockPileCoveredCards();
    expectGetCardsInHand(noCards);
    EXPECT_CALL(*graphicsSystemMock, renderFrame());
    renderer.render();
}

TEST_F(CreatedRendererTests, renderStockPileWithUncoveredCards) {
    const unsigned pileCardsSize = 5;
    const unsigned selectedCardIndex = pileCardsSize - 1;
    const auto pileCards = getPileCardsWithSelectedFiveDiamond(
        pileCardsSize, selectedCardIndex);

    expectGetStockPileData(pileCards, selectedCardIndex);
    expectGetStockPileCollider();
    expectRenderCardPlaceholder(stockPilePosition);
    expectRenderStockPileUncoveredCards();
    expectGetCardsInHand(noCards);
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer.render();
}

TEST_F(CreatedRendererTests, renderStockPileWithCoveredAndUncoveredCards) {
    const unsigned pileCardsSize = 5;
    const unsigned selectedCardIndex = 2;
    const auto pileCards = getPileCardsWithSelectedFiveDiamond(
        pileCardsSize, selectedCardIndex);

    expectGetStockPileData(pileCards, selectedCardIndex);
    expectGetStockPileCollider();
    expectRenderStockPileCoveredCards();
    expectRenderStockPileUncoveredCards();
    expectGetCardsInHand(noCards);
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer.render();
}

class CardsInHandRendererTests: public CreatedRendererTests {
public:
    CardsInHandRendererTests() {
        expectGetStockPileData(noCards, std::nullopt);
        expectRenderCardPlaceholder(stockPilePosition);
    }
};

TEST_F(CardsInHandRendererTests, renderCardsInHand) {
    expectGetCardsInHand(threeCards);
    EXPECT_CALL(contextMock, getCardsInHandPosition())
        .WillOnce(Return(cardsInHandPosition));

    auto cardPosition = cardsInHandPosition;
    for (unsigned i = 0; i < threeCards.size(); ++i) {
        expectRenderCard(cardPosition, threeCards[i]);
        cardPosition.y += cardsInHandSpacing;
    }

    EXPECT_CALL(*graphicsSystemMock, renderFrame());
    renderer.render();
}

}
