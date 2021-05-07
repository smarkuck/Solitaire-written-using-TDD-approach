add_executable(SolitaireLibUnitTests)
target_include_directories(SolitaireLibUnitTests PRIVATE headers)

target_sources(SolitaireLibUnitTests PRIVATE
    sources/ApplicationTests.cpp
    sources/DefaultContextTests.cpp
    sources/DefaultSolitaireTests.cpp
    sources/LayoutTests.cpp
    sources/archivers/HistoryTrackerTests.cpp
    sources/archivers/MoveCardsOperationSnapshotCreatorTests.cpp
    sources/cards/CardTests.cpp
    sources/cards/DeckGeneratorUtils.cpp
    sources/cards/ShuffledDeckGeneratorTests.cpp
    sources/cards/SuitTests.cpp
    sources/cards/ValueTests.cpp
    sources/colliders/FoundationPileColliderTests.cpp
    sources/events/EventsDefinitionsTests.cpp
    sources/events/EventsProcessorTests.cpp
    sources/events/SDLEventsSourceTests.cpp
    sources/geometry/AreaTests.cpp
    sources/geometry/PositionTests.cpp
    sources/geometry/SizeTests.cpp
    sources/graphics/RendererTests.cpp
    sources/graphics/SDLGraphicsSystemTests.cpp
    sources/piles/DefaultStockPileTests.cpp
    sources/piles/DefaultTableauPileTests.cpp
    sources/piles/FoundationPileTests.cpp
    sources/time/ChronoFPSLimiterTests.cpp
)

target_link_libraries(SolitaireLibUnitTests PRIVATE SolitaireLib mock_ptr gmock_main)
