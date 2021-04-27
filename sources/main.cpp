#include <cassert>
#include <string>

#include "SDL.h"
#include "cards/Card.h"
#include "Solitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "piles/DefaultFoundationPile.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"

using namespace solitaire;

class Renderer {
public:
    Renderer(const Solitaire& solitaire):
        solitaire {solitaire}
    {
        SDL_Init(SDL_INIT_VIDEO);

        window = SDL_CreateWindow("Solitaire",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480, SDL_WINDOW_SHOWN
        );
        assert(window != nullptr);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        assert(renderer != nullptr);

        background = loadTexture("background.bmp");
        cardPlaceholder = loadTexture("cards/placeholder.bmp");
        assert(SDL_SetTextureAlphaMod(cardPlaceholder, 70) == 0);

        for (unsigned i = 0; i < cardsQuantity; i++)
            cards[i] = loadTexture("cards/" + std::to_string(i) + ".bmp");
        cardBack = loadTexture("cards/back.bmp");
    }

    ~Renderer() {
        SDL_Delay(2000);

        for (const auto& texture: cards)
            SDL_DestroyTexture(texture);
        SDL_DestroyTexture(cardBack);
        SDL_DestroyTexture(cardPlaceholder);
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void render() const {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, nullptr, nullptr);

        for (int i = 0; i < 8; i++) {
            SDL_Rect rect {i*75, 0, 75, 104};
            SDL_RenderCopy(renderer, cards[i], nullptr, &rect);
        }

        for (int i = 0; i < 4; i++) {
            SDL_Rect rect {i*75, 104, 75, 104};
            SDL_RenderCopy(renderer, cardPlaceholder, nullptr, &rect);
        }

        SDL_Rect rect {0, 208, 75, 104};
        SDL_RenderCopy(renderer, cardBack, nullptr, &rect);

        SDL_RenderPresent(renderer);
    }

private:
    SDL_Texture* loadTexture(const std::string& path) const
    {
        SDL_Surface* surface = SDL_LoadBMP((assetsPath + path).c_str());
        assert(surface != nullptr);
        assert(SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 255)) == 0);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        assert(texture != nullptr);
        SDL_FreeSurface(surface);

        return texture;
    }

    static constexpr unsigned cardsQuantity {52};
    static const std::string assetsPath;

    const Solitaire& solitaire;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::array<SDL_Texture*, cardsQuantity> cards;
    SDL_Texture* cardBack;
    SDL_Texture* cardPlaceholder;
    SDL_Texture* background;
};

const std::string Renderer::assetsPath {"../../assets/"};

int main(int, char **) {
    std::array<std::shared_ptr<piles::FoundationPile>, 4> foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<piles::DefaultFoundationPile>();

    std::array<std::shared_ptr<piles::TableauPile>, 7> tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<piles::DefaultTableauPile>();

    Solitaire solitaire {
        std::make_unique<cards::ShuffledDeckGenerator>(),
        std::make_shared<piles::DefaultStockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<archivers::DefaultHistoryTracker>(),
        std::make_unique<archivers::DefaultMoveCardsOperationSnapshotCreator>()
    };

    solitaire.startNewGame();
    Renderer {solitaire}.render();

    return 0;
}
