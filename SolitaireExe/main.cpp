#include <cassert>
#include <filesystem>
#include <memory>

#include "Application.h"
#include "DefaultSolitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "events/SDLEventsSource.h"
#include "graphics/DefaultRenderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/DefaultFoundationPile.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"
#include "SDL/DefaultWrapper.h"

using namespace solitaire;

std::string findAssetsPath() {
    std::string path = "assets/";
    const std::string moveUp = "../";

    for (unsigned i = 0; i < 4; ++i, path = moveUp + path)
        if (std::filesystem::exists(path))
            return path;

    assert(false and "cannot find assets path");
    return {};
}

int main(int, char**) {
    Solitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<piles::DefaultFoundationPile>();

    Solitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<piles::DefaultTableauPile>();

    auto solitaire = std::make_unique<DefaultSolitaire>(
        std::make_unique<cards::ShuffledDeckGenerator>(),
        std::make_shared<piles::DefaultStockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<archivers::DefaultHistoryTracker>(),
        std::make_unique<archivers::DefaultMoveCardsOperationSnapshotCreator>()
    );

    auto eventsSource = std::make_unique<events::SDLEventsSource>(
        std::make_unique<SDL::DefaultWrapper>()
    );

    auto renderer = std::make_unique<graphics::DefaultRenderer>(
        *solitaire,
        std::make_unique<graphics::SDLGraphicsSystem>(
            std::make_unique<SDL::DefaultWrapper>()
        ),
        findAssetsPath()
    );

    Application {std::move(solitaire), std::move(eventsSource), std::move(renderer)}.run();
    return 0;
}
