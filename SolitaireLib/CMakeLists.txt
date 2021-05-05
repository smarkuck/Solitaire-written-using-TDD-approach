add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC headers)

target_sources(SolitaireLib PRIVATE
    sources/Application.cpp
    sources/DefaultSolitaire.cpp
    sources/archivers/DefaultHistoryTracker.cpp
    sources/archivers/DefaultMoveCardsOperationSnapshotCreator.cpp
    sources/cards/Card.cpp
    sources/cards/ShuffledDeckGenerator.cpp
    sources/events/DefaultEventsProcessor.cpp
    sources/events/SDLEventsSource.cpp
    sources/graphics/DefaultRenderer.cpp
    sources/graphics/SDLGraphicsSystem.cpp
    sources/piles/DefaultFoundationPile.cpp
    sources/piles/DefaultStockPile.cpp
    sources/piles/DefaultTableauPile.cpp
    sources/SDL/DefaultWrapper.cpp
    sources/SDL/PtrDeleter.cpp
    sources/time/ChronoFPSLimiter.cpp
    sources/time/DefaultStdTimeFunctionsWrapper.cpp
)

target_link_libraries(SolitaireLib PUBLIC Boost SDL2)

add_subdirectory(unitTests)
