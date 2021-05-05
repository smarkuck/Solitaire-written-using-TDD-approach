#include <array>

#include "ContextMock.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "archivers/Snapshot.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "graphics/DefaultRenderer.h"
#include "graphics/GraphicsSystemMock.h"
#include "graphics/TextureArea.h"
#include "piles/FoundationPileMock.h"
#include "piles/PileId.h"
#include "piles/StockPileMock.h"
#include "piles/TableauPileMock.h"

using namespace testing;
using namespace solitaire::cards;
using namespace solitaire::piles;

namespace solitaire::graphics {

namespace {
const std::string assetsPath {"dummyPath/"};
const std::string windowTitle {"Solitaire"};

constexpr unsigned windowWidth {640};
constexpr unsigned windowHeight {480};
constexpr unsigned cardPlaceholderAlpha {70};
constexpr unsigned foundationPilesCount {4};
constexpr unsigned tableauPilesCount {7};
constexpr unsigned coveredTableauPileCardsSpacing {3};
constexpr unsigned uncoveredTableauPileCardsSpacing {16};
constexpr unsigned stockPileCardsSpacing {2};

const TextureId backgroundTextureId {0};
const TextureId cardsTextureId {1};
const TextureId cardPlaceholderTextureId {2};

constexpr TextureSize cardSize {75, 104};

constexpr TextureArea cardPlaceholderTextureArea {TexturePosition {0, 0}, cardSize};
constexpr TextureArea cardBackTextureArea {TexturePosition {0, 416}, cardSize};

constexpr std::array<TexturePosition, foundationPilesCount> foundationPilesPositions {
    TexturePosition {283, 30},
    TexturePosition {372, 30},
    TexturePosition {461, 30},
    TexturePosition {550, 30}
};

constexpr std::array<TexturePosition, tableauPilesCount> tableauPilesPositions {
    TexturePosition {16, 144},
    TexturePosition {105, 144},
    TexturePosition {194, 144},
    TexturePosition {283, 144},
    TexturePosition {372, 144},
    TexturePosition {461, 144},
    TexturePosition {550, 144}
};

constexpr TexturePosition stockPilePosition {16, 30};
constexpr TexturePosition stockPileUncoveredCardsPosition {105, 30};
constexpr TexturePosition cardsInHandPosition {15, 22};

const Card fiveDiamond {Value::Five, Suit::Diamond};

const Cards noCards;
const Cards aceHeart {Card {Value::Ace, Suit::Heart}};

const Cards twoCardsWithLastSevenSpade {
    Card {Value::Ace, Suit::Heart},
    Card {Value::Seven, Suit::Spade}
};

const Cards threeCardsWithLastKingClub {
    Card {Value::Ace, Suit::Heart},
    Card {Value::Seven, Suit::Spade},
    Card {Value::King, Suit::Club}
};

const Cards sixCards {6};
const Cards sevenCards {7};
const Cards nineteenCards {19};
}

class DefaultRendererTests: public Test {
public:
    DefaultRendererTests() {
        InSequence seq;
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
    }

    SolitaireMock solitaireMock;
    ContextMock contextMock;
    mock_ptr<GraphicsSystemMock> graphicsSystemMock;
};

TEST_F(DefaultRendererTests, onConstructionShouldCreateWindowAndLoadAllTextures) {
    DefaultRenderer {solitaireMock, contextMock,
                     graphicsSystemMock.make_unique(), assetsPath};
}

class CreatedDefaultRendererTests: public DefaultRendererTests {
public:
    using TopCoveredCardPosition = unsigned;

    CreatedDefaultRendererTests() {
        renderer = std::make_unique<DefaultRenderer>(
            solitaireMock, contextMock, graphicsSystemMock.make_unique(), assetsPath);

        InSequence seq;
        EXPECT_CALL(*graphicsSystemMock, renderTextureInFullWindow(backgroundTextureId));

        expectRenderFoundationPile(PileId {0}, noCards);
        expectRenderFoundationPile(PileId {1}, aceHeart);
        expectRenderFoundationPile(PileId {2}, twoCardsWithLastSevenSpade);
        expectRenderFoundationPile(PileId {3}, threeCardsWithLastKingClub);

        expectRenderTableauPile(PileId {0}, noCards);
        expectRenderTableauPile(PileId {1}, aceHeart, TopCoveredCardPosition {0});
        expectRenderTableauPile(PileId {2}, aceHeart, TopCoveredCardPosition {1});
        expectRenderTableauPile(PileId {3}, threeCardsWithLastKingClub, TopCoveredCardPosition {0});
        expectRenderTableauPile(PileId {4}, threeCardsWithLastKingClub, TopCoveredCardPosition {1});
        expectRenderTableauPile(PileId {5}, threeCardsWithLastKingClub, TopCoveredCardPosition {2});
        expectRenderTableauPile(PileId {6}, threeCardsWithLastKingClub, TopCoveredCardPosition {3});
    }

    void expectRenderFoundationPile(const PileId id, const Cards& pileCards) {
        EXPECT_CALL(solitaireMock, getFoundationPile(id))
            .WillOnce(ReturnRef(foundationPileMocks[id]));

        EXPECT_CALL(foundationPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));

        if (pileCards.empty())
            expectRenderCardPlaceholder(foundationPilesPositions[id]);
        else
            expectRenderCard(foundationPilesPositions[id], pileCards.back());
    }

    void expectRenderTableauPile(const PileId id, const Cards& pileCards,
                                 const TopCoveredCardPosition topCoveredCardPosition = 0)
    {
        EXPECT_CALL(solitaireMock, getTableauPile(id))
            .WillOnce(ReturnRef(tableauPileMocks[id]));

        EXPECT_CALL(tableauPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));

        if (not pileCards.empty())
            expectRenderTableauPileWithCards(id, pileCards, topCoveredCardPosition);
    }

    void expectRenderTableauPileWithCards(const PileId id, const Cards& pileCards,
                                          const TopCoveredCardPosition topCoveredCardPosition)
    {
        EXPECT_CALL(tableauPileMocks[id], getTopCoveredCardPosition())
            .WillOnce(Return(topCoveredCardPosition));

        auto cardPosition = tableauPilesPositions[id];

        for (unsigned i = 0; i < topCoveredCardPosition; ++i) {
            expectRenderCardBack(cardPosition);
            cardPosition.y += coveredTableauPileCardsSpacing;
        }

        for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i) {
            expectRenderCard(cardPosition, pileCards[i]);
            cardPosition.y += uncoveredTableauPileCardsSpacing;
        }
    }

    void expectGetStockPileData(const Cards& pileCards,
                                const std::optional<unsigned>& selectedCardIndex)
    {
        EXPECT_CALL(solitaireMock, getStockPile()).WillOnce(ReturnRef(stockPileMock));
        EXPECT_CALL(stockPileMock, getCards()).WillOnce(ReturnRef(pileCards));
        EXPECT_CALL(stockPileMock, getSelectedCardIndex()).WillOnce(Return(selectedCardIndex));
    }

    void expectRenderStockPileCoveredCards(const unsigned cardsToRender) {
        auto cardPosition = stockPilePosition;
        for (unsigned i = 0; i < cardsToRender; ++i) {
            expectRenderCardBack(cardPosition);
            cardPosition.x += stockPileCardsSpacing;
        }
    }

    void expectRenderStockPileUncoveredCards(const unsigned cardsToRender) {
        auto cardPosition = stockPileUncoveredCardsPosition;
        for (unsigned i = 0; i < cardsToRender; ++i) {
            expectRenderCard(cardPosition, fiveDiamond);
            cardPosition.x += stockPileCardsSpacing;
        }
    }

    void expectRenderCard(const TexturePosition& position, const Card& card) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardsTextureId, position, getCardTextureArea(card)));
    }

    void expectRenderCardBack(const TexturePosition& position) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardsTextureId, position, cardBackTextureArea));
    }

    void expectRenderCardPlaceholder(const TexturePosition& position) {
        EXPECT_CALL(*graphicsSystemMock, renderTexture(
            cardPlaceholderTextureId, position, cardPlaceholderTextureArea));
    }

    TextureArea getCardTextureArea(const Card& card) {
        int x = to_int(card.getValue()) * cardSize.width;
        int y = to_int(card.getSuit()) * cardSize.height;
        return TextureArea {TexturePosition{x, y}, cardSize};
    }

    Cards getPileCardsWithSelectedFiveDiamond(
        unsigned quantity, unsigned selectedCardIndex)
    {
        Cards cards {quantity - 1, Card {Value::Ace, Suit::Heart}};
        cards.insert(std::next(cards.begin(), selectedCardIndex), fiveDiamond);
        return cards;
    }

    std::unique_ptr<DefaultRenderer> renderer;
    std::array<FoundationPileMock, foundationPilesCount> foundationPileMocks;
    std::array<TableauPileMock, tableauPilesCount> tableauPileMocks;
    StockPileMock stockPileMock;
};

TEST_F(CreatedDefaultRendererTests, renderSolitaireWithEmptyStockPile) {
    InSequence seq;
    expectGetStockPileData(noCards, std::nullopt);
    expectRenderCardPlaceholder(stockPilePosition);
    EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());
    renderer->render();
}

TEST_F(CreatedDefaultRendererTests, throwOnInvalidStockPileSelectedCardIndex) {
    InSequence seq;
    expectGetStockPileData(threeCardsWithLastKingClub, 3);
    EXPECT_THROW(renderer->render(), std::runtime_error);
}

struct StockPileRenderData {
    unsigned pileCardsCount;
    unsigned cardsToRender;
};

class StockPileDefaultRendererTests:
    public CreatedDefaultRendererTests,
    public testing::WithParamInterface<StockPileRenderData>
{
};

TEST_P(StockPileDefaultRendererTests, renderStockPileWithCoveredCards) {
    const auto& stockPileRenderData = GetParam();
    const Cards pileCards {stockPileRenderData.pileCardsCount};

    InSequence seq;
    expectGetStockPileData(pileCards, std::nullopt);
    expectRenderStockPileCoveredCards(stockPileRenderData.cardsToRender);
    EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer->render();
}

TEST_P(StockPileDefaultRendererTests, renderStockPileWithUncoveredCards) {
    const auto& stockPileRenderData = GetParam();
    const unsigned selectedCardIndex = stockPileRenderData.pileCardsCount - 1;
    const auto pileCards = getPileCardsWithSelectedFiveDiamond(
        stockPileRenderData.pileCardsCount, selectedCardIndex);

    InSequence seq;
    expectGetStockPileData(pileCards, selectedCardIndex);
    expectRenderCardPlaceholder(stockPilePosition);
    expectRenderStockPileUncoveredCards(stockPileRenderData.cardsToRender);
    EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer->render();
}

INSTANTIATE_TEST_SUITE_P(BorderPoints,
                         StockPileDefaultRendererTests,
                         Values(StockPileRenderData {1, 1},
                                StockPileRenderData {6, 1},
                                StockPileRenderData {7, 2},
                                StockPileRenderData {12, 2},
                                StockPileRenderData {13, 3},
                                StockPileRenderData {18, 3},
                                StockPileRenderData {19, 4}));

struct StockPileWithSelectedCardRenderData {
    unsigned pileCardsCount;
    unsigned selectedCardIndex;
    unsigned coveredCardsToRender;
    unsigned uncoveredCardsToRender;
};

class StockPileWithCoveredAndUncoveredCardsDefaultRendererTests:
    public CreatedDefaultRendererTests,
    public testing::WithParamInterface<StockPileWithSelectedCardRenderData>
{
};

TEST_P(StockPileWithCoveredAndUncoveredCardsDefaultRendererTests, renderStockPile) {
    const auto& stockPileRenderData = GetParam();
    const auto pileCards = getPileCardsWithSelectedFiveDiamond(
        stockPileRenderData.pileCardsCount, stockPileRenderData.selectedCardIndex);

    InSequence seq;
    expectGetStockPileData(pileCards, stockPileRenderData.selectedCardIndex);
    expectRenderStockPileCoveredCards(stockPileRenderData.coveredCardsToRender);
    expectRenderStockPileUncoveredCards(stockPileRenderData.uncoveredCardsToRender);
    EXPECT_CALL(solitaireMock, getCardsInHand()).WillOnce(ReturnRef(noCards));
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer->render();
}

INSTANTIATE_TEST_SUITE_P(BorderPoints,
                         StockPileWithCoveredAndUncoveredCardsDefaultRendererTests,
                         Values(StockPileWithSelectedCardRenderData {24, 10, 3, 2},
                                StockPileWithSelectedCardRenderData {24, 11, 2, 2},
                                StockPileWithSelectedCardRenderData {24, 12, 2, 3},
                                StockPileWithSelectedCardRenderData {15, 1, 3, 1},
                                StockPileWithSelectedCardRenderData {15, 2, 2, 1},
                                StockPileWithSelectedCardRenderData {15, 5, 2, 1},
                                StockPileWithSelectedCardRenderData {15, 6, 2, 2}));

class CardsInHandDefaultRendererTests: public CreatedDefaultRendererTests {
public:
    CardsInHandDefaultRendererTests() {
        InSequence seq;
        expectGetStockPileData(noCards, std::nullopt);
        expectRenderCardPlaceholder(stockPilePosition);
    }
};

TEST_F(CardsInHandDefaultRendererTests, renderCardsInHand) {
    EXPECT_CALL(solitaireMock, getCardsInHand())
        .WillOnce(ReturnRef(threeCardsWithLastKingClub));
    EXPECT_CALL(contextMock, getCardsInHandPosition())
        .WillOnce(Return(cardsInHandPosition));

    auto cardPosition = cardsInHandPosition;
    for (unsigned i = 0; i < threeCardsWithLastKingClub.size(); ++i) {
        expectRenderCard(cardPosition, threeCardsWithLastKingClub[i]);
        cardPosition.y += uncoveredTableauPileCardsSpacing;
    }

    EXPECT_CALL(*graphicsSystemMock, renderFrame());
    renderer->render();
}

}
