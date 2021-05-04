add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC headers)

target_sources(SolitaireLib PRIVATE
    sources/DefaultSolitaire.cpp
    sources/archivers/DefaultHistoryTracker.cpp
    sources/archivers/DefaultMoveCardsOperationSnapshotCreator.cpp
    sources/cards/Card.cpp
    sources/cards/ShuffledDeckGenerator.cpp
    sources/events/SDLEventsSource.cpp
    sources/graphics/Renderer.cpp
    sources/graphics/SDLGraphicsSystem.cpp
    sources/piles/DefaultFoundationPile.cpp
    sources/piles/DefaultStockPile.cpp
    sources/piles/DefaultTableauPile.cpp
    sources/SDL/DefaultWrapper.cpp
    sources/SDL/PtrDeleter.cpp
)

target_link_libraries(SolitaireLib PUBLIC Boost SDL2)

add_subdirectory(unitTests)
