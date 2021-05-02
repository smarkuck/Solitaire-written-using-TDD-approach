#include <array>

#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "archivers/Snapshot.h"
#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gmock/gmock.h"
#include "graphics/GraphicsSystemMock.h"
#include "graphics/Renderer.h"
#include "graphics/TextureArea.h"
#include "piles/FoundationPileMock.h"
#include "piles/PileId.h"
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

const TextureId backgroundTextureId {0};
const TextureId cardsTextureId {1};
const TextureId cardPlaceholderTextureId {2};

constexpr TextureSize cardSize {75, 104};

constexpr TextureArea cardPlaceholderTextureArea {
    TexturePosition {0, 0}, cardSize};

constexpr TextureArea cardBackTextureArea {
    TexturePosition {0, 416}, cardSize};

const std::array<TexturePosition, foundationPilesCount> foundationPilesPositions {
    TexturePosition {283, 30},
    TexturePosition {372, 30},
    TexturePosition {461, 30},
    TexturePosition {550, 30}
};

const std::array<TexturePosition, tableauPilesCount> tableauPilesPositions {
    TexturePosition {16, 144},
    TexturePosition {105, 144},
    TexturePosition {194, 144},
    TexturePosition {283, 144},
    TexturePosition {372, 144},
    TexturePosition {461, 144},
    TexturePosition {550, 144}
};

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
}

class RendererTests: public Test {
public:
    using TopCoveredCardPosition = unsigned;

    std::array<FoundationPileMock, foundationPilesCount> foundationPileMocks;
    std::array<TableauPileMock, tableauPilesCount> tableauPileMocks;
    SolitaireMock solitaireMock;
    mock_ptr<GraphicsSystemMock> graphicsSystemMock;
    std::unique_ptr<Renderer> renderer;

    RendererTests() {
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

        renderer = std::make_unique<Renderer>(
            solitaireMock, graphicsSystemMock.make_unique(), assetsPath);
    }

    void expectRenderFoundationPile(const PileId id, const Cards& pileCards) {
        EXPECT_CALL(solitaireMock, getFoundationPile(id))
            .WillOnce(ReturnRef(foundationPileMocks[id]));

        EXPECT_CALL(foundationPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));

        if (pileCards.empty())
            EXPECT_CALL(*graphicsSystemMock,
                renderTexture(cardPlaceholderTextureId,
                    foundationPilesPositions[id], cardPlaceholderTextureArea));
        else
            EXPECT_CALL(*graphicsSystemMock,
                renderTexture(cardsTextureId, foundationPilesPositions[id],
                    getCardTextureArea(pileCards.back())));
    }

    void expectRenderTableauPile(const PileId id, const Cards& pileCards,
                                 const TopCoveredCardPosition topCoveredCardPosition = 0)
    {
        EXPECT_CALL(solitaireMock, getTableauPile(id))
            .WillOnce(ReturnRef(tableauPileMocks[id]));

        EXPECT_CALL(tableauPileMocks[id], getCards())
            .WillOnce(ReturnRef(pileCards));

        if (pileCards.empty())
            expectRenderEmptyTableauPile(id);
        else
            expectRenderTableauPileWithCards(id, pileCards, topCoveredCardPosition);
    }

    void expectRenderEmptyTableauPile(const PileId id) {
        EXPECT_CALL(*graphicsSystemMock,
            renderTexture(cardPlaceholderTextureId,
                tableauPilesPositions[id], cardPlaceholderTextureArea));
    }

    void expectRenderTableauPileWithCards(const PileId id, const Cards& pileCards,
                                          const TopCoveredCardPosition topCoveredCardPosition)
    {
        EXPECT_CALL(tableauPileMocks[id], getTopCoveredCardPosition())
            .WillOnce(Return(topCoveredCardPosition));

        auto cardPosition = tableauPilesPositions[id];

        for (unsigned i = 0; i < topCoveredCardPosition; ++i) {
            EXPECT_CALL(*graphicsSystemMock,
                renderTexture(cardsTextureId, cardPosition, cardBackTextureArea));
            cardPosition.y += coveredTableauPileCardsSpacing;
        }

        for (unsigned i = topCoveredCardPosition; i < pileCards.size(); ++i) {
            EXPECT_CALL(*graphicsSystemMock, renderTexture(
                    cardsTextureId, cardPosition, getCardTextureArea(pileCards[i])));
            cardPosition.y += uncoveredTableauPileCardsSpacing;
        }
    }

    TextureArea getCardTextureArea(const Card& card) {
        unsigned x = to_int(card.getValue()) * cardSize.width;
        unsigned y = to_int(card.getSuit()) * cardSize.height;
        return TextureArea {TexturePosition{x, y}, cardSize};
    }
};

TEST_F(RendererTests, onConstructionShouldCreateWindowAndLoadAllTextures) {}

TEST_F(RendererTests, renderFoundationAndTableauPiles) {
    InSequence seq;
    EXPECT_CALL(*graphicsSystemMock, renderTextureOnFullscreen(backgroundTextureId));
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

    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer->render();
}

}
