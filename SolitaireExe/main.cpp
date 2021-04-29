#include "cards/Card.h"
#include "Solitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "graphics/DefaultSDLWrapper.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/Renderer.h"
#include "piles/DefaultFoundationPile.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"

using namespace solitaire;

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

    graphics::GraphicsSystem system {std::make_unique<graphics::DefaultSDLWrapper>()};
    graphics::Renderer renderer {solitaire, system};
    for (unsigned i = 0; i < 150; ++i)
        renderer.render();

    return 0;
}
