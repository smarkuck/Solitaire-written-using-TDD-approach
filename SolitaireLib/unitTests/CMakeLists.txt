add_executable(SolitaireLibUnitTests)
target_include_directories(SolitaireLibUnitTests PRIVATE headers)

target_sources(SolitaireLibUnitTests PRIVATE
    sources/SolitaireTests.cpp
    sources/cards/CardTests.cpp
    sources/cards/CardsGeneratorUtils.cpp
    sources/cards/ShuffledCardsGeneratorTests.cpp
    sources/piles/DefaultFoundationPileTests.cpp
    sources/piles/DefaultStockPileTests.cpp
    sources/piles/DefaultTableauPileTests.cpp
)

target_link_libraries(SolitaireLibUnitTests PRIVATE SolitaireLib mock_ptr gmock_main)
