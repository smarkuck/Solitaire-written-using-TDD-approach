add_executable(SolitaireLibUnitTests)

target_sources(SolitaireLibUnitTests PRIVATE
    sources/cards/CardTests.cpp
    sources/cards/ShuffledCardsGeneratorTests.cpp
    sources/piles/DefaultFoundationPileTests.cpp
    sources/piles/DefaultStockPileTests.cpp
    sources/piles/DefaultTableauPileTests.cpp
)

target_link_libraries(SolitaireLibUnitTests PRIVATE SolitaireLib mock_ptr gmock_main)
