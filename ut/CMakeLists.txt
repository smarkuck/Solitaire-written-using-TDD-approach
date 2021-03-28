add_executable(UnitTests)

target_sources(UnitTests PRIVATE
    card/CardTest.cpp
    piles/TableauPileTest.cpp
)

target_link_libraries(UnitTests PRIVATE gmock_main)
