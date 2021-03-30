add_executable(UnitTests)

target_sources(UnitTests PRIVATE
    cards/CardTest.cpp
    cards/ShuffledCardsGeneratorTest.cpp
    piles/FoundationPileTest.cpp
    piles/StockPileTest.cpp
    piles/TableauPileTest.cpp
)

target_link_libraries(UnitTests PRIVATE ${SOLITAIRE_LIB} gmock_main)
