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

const TextureId backgroundTextureId {0};
const TextureId cardsTextureId {1};
const TextureId cardPlaceholderTextureId {2};

constexpr TextureSize cardSize {75, 104};

constexpr TextureArea cardPlaceholderTextureArea {
    TexturePosition {0, 0}, cardSize};

const std::array<TexturePosition, foundationPilesCount> foundationPilesPositions {
    TexturePosition {283, 30},
    TexturePosition {372, 30},
    TexturePosition {461, 30},
    TexturePosition {550, 30},
};

const Cards noCards;
const Cards aceHeart {Card {Value::Ace, Suit::Heart}};

const Cards cardsWithLastSevenSpade {
    Card {Value::Ace, Suit::Heart},
    Card {Value::Seven, Suit::Spade}
};

const Cards cardsWithLastKingClub {
    Card {Value::Ace, Suit::Heart},
    Card {Value::King, Suit::Club}
};
}

class RendererTests: public Test {
public:
    std::array<FoundationPileMock, foundationPilesCount> foundationPileMocks;
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

    TextureArea getCardTextureArea(const Card& card) {
        unsigned x = to_int(card.getValue()) * cardSize.width;
        unsigned y = to_int(card.getSuit()) * cardSize.height;
        return TextureArea {x, y, cardSize};
    }
};

TEST_F(RendererTests, onConstructionShouldCreateWindowAndLoadAllTextures) {}

TEST_F(RendererTests, renderFoundationPiles) {
    InSequence seq;
    EXPECT_CALL(*graphicsSystemMock, renderTextureOnFullscreen(backgroundTextureId));
    expectRenderFoundationPile(PileId {0}, noCards);
    expectRenderFoundationPile(PileId {1}, aceHeart);
    expectRenderFoundationPile(PileId {2}, cardsWithLastSevenSpade);
    expectRenderFoundationPile(PileId {3}, cardsWithLastKingClub);
    EXPECT_CALL(*graphicsSystemMock, renderFrame());

    renderer->render();
}

}
