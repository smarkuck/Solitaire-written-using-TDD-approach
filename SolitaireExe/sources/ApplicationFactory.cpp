#include <cassert>
#include <filesystem>
#include <memory>

#include "Application.h"
#include "ApplicationFactory.h"
#include "DefaultSolitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "events/SDLEventsSource.h"
#include "events/DefaultEventsProcessor.h"
#include "graphics/DefaultRenderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/DefaultFoundationPile.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"
#include "SDL/DefaultWrapper.h"
#include "time/ChronoFPSLimiter.h"
#include "time/DefaultStdTimeFunctionsWrapper.h"

using namespace solitaire;
using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::events;
using namespace solitaire::graphics;
using namespace solitaire::piles;
using namespace solitaire::time;

Application ApplicationFactory::make() const {
    auto solitaire = makeSolitaire();
    auto renderer = makeRenderer(*solitaire);
    auto eventsProcessor = makeEventsProcessor(*solitaire);

    return Application {std::move(solitaire), std::move(eventsProcessor),
                        std::move(renderer), makeFPSLimiter()};
}

std::unique_ptr<Solitaire> ApplicationFactory::makeSolitaire() const {
    Solitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<DefaultFoundationPile>();

    Solitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<DefaultTableauPile>();

    return std::make_unique<DefaultSolitaire>(
        std::make_unique<ShuffledDeckGenerator>(),
        std::make_shared<DefaultStockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<DefaultHistoryTracker>(),
        std::make_unique<DefaultMoveCardsOperationSnapshotCreator>()
    );
}

std::unique_ptr<EventsProcessor>
ApplicationFactory::makeEventsProcessor(Solitaire& solitaire) const
{
    return std::make_unique<DefaultEventsProcessor>(
        solitaire,
        std::make_unique<SDLEventsSource>(
            std::make_unique<SDL::DefaultWrapper>()
        )
    );
}

std::unique_ptr<Renderer> ApplicationFactory::makeRenderer(
    const Solitaire& solitaire) const
{
    return std::make_unique<DefaultRenderer>(
        solitaire,
        std::make_unique<SDLGraphicsSystem>(
            std::make_unique<SDL::DefaultWrapper>()
        ),
        findAssetsPath()
    );
}

std::unique_ptr<FPSLimiter> ApplicationFactory::makeFPSLimiter() const {
    return std::make_unique<ChronoFPSLimiter>(
        std::make_unique<DefaultStdTimeFunctionsWrapper>()
    );
}

std::string ApplicationFactory::findAssetsPath() const {
    std::string path = "assets/";
    const std::string moveUp = "../";

    for (unsigned i = 0; i < 4; ++i, path = moveUp + path)
        if (std::filesystem::exists(path))
            return path;

    assert(false and "cannot find assets path");
    return {};
}