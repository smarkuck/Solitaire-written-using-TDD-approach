add_executable(SolitaireLibUnitTests)
target_include_directories(SolitaireLibUnitTests PRIVATE headers)

target_sources(SolitaireLibUnitTests PRIVATE
    sources/DefaultSolitaireTests.cpp
    sources/archivers/DefaultHistoryTrackerTests.cpp
    sources/archivers/DefaultMoveCardsOperationSnapshotCreatorTests.cpp
    sources/cards/CardTests.cpp
    sources/cards/DeckGeneratorUtils.cpp
    sources/cards/ShuffledDeckGeneratorTests.cpp
    sources/graphics/RendererTests.cpp
    sources/graphics/SDLGraphicsSystemTests.cpp
    sources/piles/DefaultFoundationPileTests.cpp
    sources/piles/DefaultStockPileTests.cpp
    sources/piles/DefaultTableauPileTests.cpp
)

target_link_libraries(SolitaireLibUnitTests PRIVATE SolitaireLib mock_ptr gmock_main)
