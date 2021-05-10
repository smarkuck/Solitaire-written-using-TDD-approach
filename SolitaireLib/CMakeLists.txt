add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC headers)

target_sources(SolitaireLib PRIVATE
    sources/Application.cpp
    sources/Button.cpp
    sources/Context.cpp
    sources/Layout.cpp
    sources/Solitaire.cpp
    sources/archivers/HistoryTracker.cpp
    sources/archivers/MoveCardsOperationSnapshotCreator.cpp
    sources/cards/Card.cpp
    sources/cards/ShuffledDeckGenerator.cpp
    sources/colliders/FoundationPileCollider.cpp
    sources/colliders/StockPileCollider.cpp
    sources/colliders/TableauPileCollider.cpp
    sources/events/EventsProcessor.cpp
    sources/events/SDLEventsSource.cpp
    sources/graphics/Renderer.cpp
    sources/graphics/SDLGraphicsSystem.cpp
    sources/piles/FoundationPile.cpp
    sources/piles/StockPile.cpp
    sources/piles/TableauPile.cpp
    sources/SDL/PtrDeleter.cpp
    sources/SDL/Wrapper.cpp
    sources/time/ChronoFPSLimiter.cpp
    sources/time/StdTimeFunctionsWrapper.cpp
)

target_link_libraries(SolitaireLib PUBLIC Boost SDL2)

add_subdirectory(unitTests)
