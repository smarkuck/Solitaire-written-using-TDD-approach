#include <cassert>
#include <filesystem>
#include <memory>

#include "Application.h"
#include "ApplicationFactory.h"
#include "DefaultContext.h"
#include "DefaultSolitaire.h"
#include "Layout.h"
#include "archivers/HistoryTracker.h"
#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "colliders/FoundationPileCollider.h"
#include "events/EventsProcessor.h"
#include "events/SDLEventsSource.h"
#include "graphics/Renderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/DefaultStockPile.h"
#include "piles/DefaultTableauPile.h"
#include "piles/FoundationPile.h"
#include "piles/PileId.h"
#include "SDL/DefaultWrapper.h"
#include "time/ChronoFPSLimiter.h"
#include "time/DefaultStdTimeFunctionsWrapper.h"

using namespace solitaire;
using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::colliders;
using namespace solitaire::events;
using namespace solitaire::graphics;
using namespace solitaire::interfaces;
using namespace solitaire::piles;
using namespace solitaire::time;
using namespace solitaire::time::interfaces;

Application ApplicationFactory::make() const {
    auto context = makeContext();
    auto eventsProcessor = makeEventsProcessor(*context);
    auto renderer = makeRenderer(*context);

    return Application {std::move(context), std::move(eventsProcessor),
                        std::move(renderer), makeFPSLimiter()};
}

std::unique_ptr<Context> ApplicationFactory::makeContext() const {
    DefaultContext::FoundationPileColliders foundationPileColliders;
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        foundationPileColliders[id] =
            std::make_unique<FoundationPileCollider>(
                Layout::getFoundationPilePosition(id));

    return std::make_unique<DefaultContext>(
        makeSolitaire(), std::move(foundationPileColliders));
}

std::unique_ptr<Solitaire> ApplicationFactory::makeSolitaire() const {
    Solitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<FoundationPile>();

    Solitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<DefaultTableauPile>();

    return std::make_unique<DefaultSolitaire>(
        std::make_unique<ShuffledDeckGenerator>(),
        std::make_shared<DefaultStockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<HistoryTracker>(),
        std::make_unique<MoveCardsOperationSnapshotCreator>()
    );
}

std::unique_ptr<events::interfaces::EventsProcessor>
ApplicationFactory::makeEventsProcessor(Context& context) const {
    return std::make_unique<EventsProcessor>(
        context,
        std::make_unique<SDLEventsSource>(
            std::make_unique<SDL::DefaultWrapper>()
        )
    );
}

std::unique_ptr<graphics::interfaces::Renderer>
ApplicationFactory::makeRenderer(const Context& context) const {
    return std::make_unique<Renderer>(
        context,
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