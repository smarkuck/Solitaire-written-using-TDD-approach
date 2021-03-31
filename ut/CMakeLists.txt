add_executable(UnitTests)

target_sources(UnitTests PRIVATE
    cards/CardTest.cpp
    cards/ShuffledCardsGeneratorTest.cpp
    piles/DefaultFoundationPileTest.cpp
    piles/DefaultStockPileTest.cpp
    piles/DefaultTableauPileTest.cpp
)

target_link_libraries(UnitTests PRIVATE ${SOLITAIRE_LIB} gmock_main)
