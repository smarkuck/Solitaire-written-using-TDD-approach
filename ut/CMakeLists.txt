add_executable(UnitTests)

target_sources(UnitTests PRIVATE
    src/cards/CardTest.cpp
    src/cards/ShuffledCardsGeneratorTest.cpp
    src/piles/DefaultFoundationPileTest.cpp
    src/piles/DefaultStockPileTest.cpp
    src/piles/DefaultTableauPileTest.cpp
)

target_link_libraries(UnitTests PRIVATE SolitaireLib mock_ptr gmock_main)
