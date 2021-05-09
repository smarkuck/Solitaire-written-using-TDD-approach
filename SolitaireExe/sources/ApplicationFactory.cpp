#include <cassert>
#include <filesystem>
#include <memory>

#include "Application.h"
#include "ApplicationFactory.h"
#include "Context.h"
#include "Layout.h"
#include "Solitaire.h"
#include "archivers/HistoryTracker.h"
#include "archivers/MoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "colliders/FoundationPileCollider.h"
#include "colliders/StockPileCollider.h"
#include "colliders/TableauPileCollider.h"
#include "events/EventsProcessor.h"
#include "events/SDLEventsSource.h"
#include "graphics/Renderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/FoundationPile.h"
#include "piles/StockPile.h"
#include "piles/TableauPile.h"
#include "piles/PileId.h"
#include "SDL/Wrapper.h"
#include "time/ChronoFPSLimiter.h"
#include "time/StdTimeFunctionsWrapper.h"

using namespace solitaire;
using namespace solitaire::archivers;
using namespace solitaire::cards;
using namespace solitaire::colliders;
using namespace solitaire::events;
using namespace solitaire::graphics;
using namespace solitaire::piles;
using namespace solitaire::time;

Application ApplicationFactory::make() const {
    auto context = makeContext();
    auto eventsProcessor = makeEventsProcessor(*context);
    auto renderer = makeRenderer(*context);

    return Application {std::move(context), std::move(eventsProcessor),
                        std::move(renderer), makeFPSLimiter()};
}

std::unique_ptr<solitaire::interfaces::Context>
ApplicationFactory::makeContext() const
{
    auto solitaire = makeSolitaire();

    Context::FoundationPileColliders foundationPileColliders;
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        foundationPileColliders[id] =
            std::make_unique<FoundationPileCollider>(
                Layout::getFoundationPilePosition(id));

    Context::TableauPileColliders tableauPileColliders;
    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id)
        tableauPileColliders[id] =
            std::make_unique<TableauPileCollider>(
                Layout::getTableauPilePosition(id), solitaire->getTableauPile(id));

    return std::make_unique<Context>(
        std::move(solitaire), std::move(foundationPileColliders),
        std::move(tableauPileColliders),
        std::make_unique<StockPileCollider>(solitaire->getStockPile()));
}

std::unique_ptr<solitaire::interfaces::Solitaire>
ApplicationFactory::makeSolitaire() const
{
    Solitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<FoundationPile>();

    Solitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<TableauPile>();

    return std::make_unique<Solitaire>(
        std::make_unique<ShuffledDeckGenerator>(),
        std::make_shared<StockPile>(),
        foundationPiles, tableauPiles,
        std::make_unique<HistoryTracker>(),
        std::make_unique<MoveCardsOperationSnapshotCreator>()
    );
}

std::unique_ptr<events::interfaces::EventsProcessor>
ApplicationFactory::makeEventsProcessor(
    solitaire::interfaces::Context& context) const
{
    return std::make_unique<EventsProcessor>(
        context,
        std::make_unique<SDLEventsSource>(
            std::make_unique<SDL::Wrapper>()
        )
    );
}

std::unique_ptr<graphics::interfaces::Renderer>
ApplicationFactory::makeRenderer(
    const solitaire::interfaces::Context& context) const
{
    return std::make_unique<Renderer>(
        context,
        std::make_unique<SDLGraphicsSystem>(
            std::make_unique<SDL::Wrapper>()
        ),
        findAssetsPath()
    );
}

std::unique_ptr<time::interfaces::FPSLimiter>
ApplicationFactory::makeFPSLimiter() const {
    return std::make_unique<ChronoFPSLimiter>(
        std::make_unique<StdTimeFunctionsWrapper>()
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