add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC headers)

target_sources(SolitaireLib PRIVATE
    sources/Solitaire.cpp
    sources/archivers/DefaultHistoryTracker.cpp
    sources/archivers/DefaultMoveCardsOperationSnapshotCreator.cpp
    sources/cards/Card.cpp
    sources/cards/ShuffledDeckGenerator.cpp
    sources/graphics/DefaultSDLWrapper.cpp
    sources/graphics/Renderer.cpp
    sources/graphics/SDLDeleter.cpp
    sources/graphics/SDLGraphicsSystem.cpp
    sources/piles/DefaultFoundationPile.cpp
    sources/piles/DefaultStockPile.cpp
    sources/piles/DefaultTableauPile.cpp
)

target_link_libraries(SolitaireLib PUBLIC Boost SDL2)

add_subdirectory(unitTests)
