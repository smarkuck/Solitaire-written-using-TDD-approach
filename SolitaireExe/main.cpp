#include "DefaultSolitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "graphics/DefaultSDLWrapper.h"
#include "graphics/Renderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/DefaultFoundationPile.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"

using namespace solitaire;

int main(int, char**) {
    DefaultSolitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<piles::DefaultFoundationPile>();

    DefaultSolitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<piles::DefaultTableauPile>();

    DefaultSolitaire solitaire {
        std::make_unique<cards::ShuffledDeckGenerator>(),
        std::make_shared<piles::DefaultStockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<archivers::DefaultHistoryTracker>(),
        std::make_unique<archivers::DefaultMoveCardsOperationSnapshotCreator>()
    };

    solitaire.startNewGame();

    graphics::Renderer renderer {
        solitaire, std::make_unique<graphics::SDLGraphicsSystem>(
            std::make_unique<graphics::DefaultSDLWrapper>()
        )
    };

    for (unsigned i = 0; i < 150; ++i)
        renderer.render();

    return 0;
}
